// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <medx11/Renderer.h>
#include <medx11/WindowsOS.h>
#include <medx11/VertexBuffer.h>
#include <medx11/IndexBuffer.h>
#include <medx11/VertexShader.h>
#include <medx11/PixelShader.h>
#include <medx11/VertexConstruct.h>
#include <medx11/Texture.h>
#include <me/RenderMethod.h>
#include <me/exception/FailedToCreate.h>
#include <me/exception/NotImplemented.h>
#include <cassert>

#include <me/MatrixFeed.h>

using namespace medx11;
using namespace me;

Renderer::Renderer( WindowsOS * os, me::Display display, size_t index )
	: m_os( os )
	, m_display( display )
	, m_swapChainDesc{}
	, m_index{ index }
	, m_totalInstances{ 5000 }
{
	bool debug =
#if defined( DEBUG ) || defined( _DEBUG )
		true;
#else
		false;
#endif

	HRESULT result = S_OK;

	HWND hWnd = (HWND)display.GetHandle();

	m_swapChainDesc.BufferCount = 1;
	m_swapChainDesc.BufferDesc.Width = (unsigned int )display.GetSize().width;
	m_swapChainDesc.BufferDesc.Height = (unsigned int)display.GetSize().height;
	m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_swapChainDesc.OutputWindow = hWnd;
	m_swapChainDesc.SampleDesc.Count = 1;
	m_swapChainDesc.SampleDesc.Quality = 0;
	m_swapChainDesc.Windowed = 1;

	unsigned int flags = debug ? D3D11_CREATE_DEVICE_DEBUG : 0;
	D3D_FEATURE_LEVEL featureLevelsRequested[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL featureLevelSupported;
	result = D3D11CreateDeviceAndSwapChain( 0, D3D_DRIVER_TYPE_HARDWARE, 0, flags, featureLevelsRequested, sizeof( featureLevelsRequested ) / sizeof( D3D_FEATURE_LEVEL ), D3D11_SDK_VERSION, &m_swapChainDesc, &m_swapChain, &m_dxDevice, &featureLevelSupported, &m_dxContext );
	if ( FAILED( result ) )
	{
		throw me::exception::FailedToCreate( "Failed to create Direct-X 11!" );
	}

	{
		// Create the back buffer...

		CComPtr< ID3D11Texture2D > backBuffer;
		result = m_swapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (void**)&backBuffer );
		if ( FAILED( result ) )
		{
			m_swapChain = 0;
			m_dxDevice = 0;
			m_dxContext = 0;
			throw exception::FailedToCreate( "Failed to get backbuffer during device creation!" );
		}

		result = m_dxDevice->CreateRenderTargetView( backBuffer, 0, &m_renderTargetView );
		backBuffer = 0;
		if ( FAILED( result ) )
		{
			m_swapChain = 0;
			m_dxDevice = 0;
			m_dxContext = 0;
			throw exception::FailedToCreate( "Failed to create render target view during device creation!" );
		}
	}

	{
		D3D11_TEXTURE2D_DESC depthStencilDesc {};
		depthStencilDesc.Width = (unsigned int)display.GetSize().width;
		depthStencilDesc.Height = (unsigned int)display.GetSize().height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;
		result = m_dxDevice->CreateTexture2D( &depthStencilDesc, nullptr, &m_depthStencilBuffer );
		assert( !FAILED( result ) );

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = D3D11_DEPTH_STENCIL_VIEW_DESC();
		depthStencilViewDesc.Format = depthStencilDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		result = m_dxDevice->CreateDepthStencilView( m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView );
		assert( !FAILED( result ) );

		m_dxContext->OMSetRenderTargets( 1, &m_renderTargetView.p, m_depthStencilView.p );
	}

	D3D11_VIEWPORT viewport {};
	{
		viewport.Width = static_cast< float >(m_swapChainDesc.BufferDesc.Width);
		viewport.Height = static_cast< float >(m_swapChainDesc.BufferDesc.Height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		m_dxContext->RSSetViewports( 1, &viewport );
	}

	{
		D3D11_RASTERIZER_DESC rasterizerDesc {};
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE; // default: D3D11_CULL_BACK
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;
		rasterizerDesc.DepthClipEnable = true;
		rasterizerDesc.ScissorEnable = false;
		rasterizerDesc.MultisampleEnable = false;
		rasterizerDesc.AntialiasedLineEnable = false;
		m_dxDevice->CreateRasterizerState( &rasterizerDesc, &m_rasterizerState );
	}
	m_dxContext->RSSetState( m_rasterizerState );

	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.ByteWidth = sizeof( unify::Matrix ) * m_totalInstances;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				  

		result = m_dxDevice->CreateBuffer( &bufferDesc, nullptr, &m_instanceBufferM[ 0 ] );
		assert( !FAILED( result ) );

		result = m_dxDevice->CreateBuffer( &bufferDesc, nullptr, &m_instanceBufferM[ 1 ] );
		assert( !FAILED( result ) );
	}
}

Renderer::~Renderer()
{
	m_instanceBufferM[ 0 ] = nullptr;
	m_instanceBufferM[ 1 ] = nullptr;
	m_dxContext = nullptr;
	m_dxDevice = nullptr;
}

me::IGame * Renderer::GetGame()
{
	return m_os->GetGame();
}

const me::Display & Renderer::GetDisplay() const
{
	return m_display;
}

ID3D11Device * Renderer::GetDxDevice() const
{
	return m_dxDevice;
}

ID3D11DeviceContext * Renderer::GetDxContext() const
{
	return m_dxContext;
}

IDXGISwapChain * Renderer::GetSwapChain() const
{
	return m_swapChain;
}
	   
void Renderer::BeforeRender()
{
	float clearColor[] = { 0.5f, 0.0f, 0.3f, 1.0f };
	m_dxContext->ClearRenderTargetView( m_renderTargetView, clearColor );
	m_dxContext->ClearDepthStencilView( m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	m_dxContext->OMSetDepthStencilState( 0, 0 );
	float blendFactors[] = { 0, 0, 0, 0 };
	m_dxContext->OMSetBlendState( 0, blendFactors, std::numeric_limits< unsigned int >::max() );
}

void Renderer::AfterRender()
{
	m_swapChain->Present( 0, 0 );
}

void Renderer::SetCullMode( me::CullMode::TYPE mode )
{
	// TODO:
}

me::Viewport Renderer::GetViewport() const
{
	return Viewport( 0, 0, GetDisplay().GetSize().width, GetDisplay().GetSize().height, GetDisplay().GetNearZ(), GetDisplay().GetFarZ() );
}

bool Renderer::IsFullscreen() const
{
	return m_display.IsFullscreen();
}

size_t Renderer::GetIndex() const
{
	return m_index;
}

void* Renderer::GetHandle() const
{
	return (HWND)m_display.GetHandle();
}
 
void Renderer::Render( const me::RenderMethod & method, const me::RenderInfo & renderInfo, MatrixFeed & matrixFeed )
{
	int instancingSlot = method.effect->GetVertexShader()->GetVertexDeclaration()->GetInstanceingSlot();
	Instancing::TYPE instancing = Instancing::None;
	if ( instancingSlot != -1 )
	{
		instancing = method.effect->GetVertexShader()->GetVertexDeclaration()->GetInstancing( instancingSlot );
	}

	D3D11_PRIMITIVE_TOPOLOGY topology{};
	switch( method.primitiveType )
	{
	case PrimitiveType::PointList: 
		topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST; 
		break;
	case PrimitiveType::LineList: 
		topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST; 
		break;
	case PrimitiveType::LineStrip: 
		topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP; 
		break;
	case PrimitiveType::TriangleList: 
		topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;	
		break;
	case PrimitiveType::TriangleStrip: 
		topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;  
		break;
	}
	m_dxContext->IASetPrimitiveTopology( topology );

	auto && vertexShader = method.effect->GetVertexShader();
	auto && constants = vertexShader->GetConstants();
	auto worldRef = constants->GetWorld();

	size_t matricesPerInstance = matrixFeed.MatricesPerInstance();	   
	size_t write = 0;	  

	switch( instancing )
	{
	case Instancing::None:
		{
			// With no instancing, we except a world matrix in the constant buffer.
			auto world = constants->GetVariable( worldRef );
			auto viewRef = constants->GetView();
			auto projRef = constants->GetProjection();

			unify::DataLock lock;

			while ( ! matrixFeed.Done() )
			{					 
				for( size_t bufferIndex = 0, buffer_count = constants->BufferCount(); bufferIndex < buffer_count; bufferIndex++ )
				{
					vertexShader->LockConstants( bufferIndex, lock );

					if ( bufferIndex == viewRef.buffer )
					{
						unsigned char * data = ((unsigned char *)lock.GetData()) + viewRef.offsetInBytes;
						unify::Matrix* matrix = (unify::Matrix*)data;
						*matrix = renderInfo.GetViewMatrix();
					}
		
					if ( bufferIndex == projRef.buffer )
					{
						unsigned char * data = ((unsigned char *)lock.GetData()) + projRef.offsetInBytes;
						unify::Matrix* matrix = (unify::Matrix*)data;
						*matrix = renderInfo.GetProjectionMatrix();
					}

					if ( bufferIndex == worldRef.buffer )
					{	
						unsigned char * data = ((unsigned char *)lock.GetData()) + worldRef.offsetInBytes;
						unify::Matrix* matrix = (unify::Matrix*)data;
						write += matrixFeed.Consume( &matrix[write], world.count );
					}

					vertexShader->UnlockConstants( bufferIndex, lock );
					bufferIndex++;
				}
				 
				method.effect->Use(); 

				if( method.useIB == false )
				{
					m_dxContext->DrawInstanced( method.vertexCount, write, method.startVertex, 0 );
				}
				else
				{
					m_dxContext->DrawIndexedInstanced( method.indexCount, write, method.startIndex, method.baseVertexIndex, 0 );
				}
				write = 0;
			}
		}
		break;
	case Instancing::Matrix:	
		{

			HRESULT result = S_OK;
			size_t bufferStride = sizeof( unify::Matrix );
			size_t offset = 0;

			method.effect->UpdateData( renderInfo, nullptr, 0 );

			unify::DataLock lock;
			D3D11_MAPPED_SUBRESOURCE subResource{};

			while ( ! matrixFeed.Done() )
			{
				result = m_dxContext->Map( m_instanceBufferM[0], 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &subResource );
				assert( !FAILED( result ) );

				write += matrixFeed.Consume( &((unify::Matrix*)subResource.pData)[write], m_totalInstances );

				size_t instanceCount = write / matricesPerInstance;
;
				
				m_dxContext->Unmap( m_instanceBufferM[0], 0 );

				method.effect->Use();

				m_dxContext->IASetVertexBuffers( 1, 1, &m_instanceBufferM[0].p, &bufferStride, &offset );

				if ( method.useIB == false )
				{
					m_dxContext->DrawInstanced( method.vertexCount, instanceCount, method.startVertex, 0 );
				}
				else
				{
					m_dxContext->DrawIndexedInstanced( method.indexCount, instanceCount, method.startIndex, method.baseVertexIndex, 0 );
				}
				write = 0;
			}
		}

		break;
	case Instancing::QP:
		break;
	}
}

me::ISketcher * Renderer::GetSketcher()
{
	return nullptr;
}

me::IVertexBuffer::ptr Renderer::ProduceVB( VertexBufferParameters parameters ) 
{
	return me::IVertexBuffer::ptr( new VertexBuffer( this, parameters ) );
}

me::IIndexBuffer::ptr Renderer::ProduceIB( IndexBufferParameters parameters ) 
{
	return me::IIndexBuffer::ptr( new IndexBuffer( this, parameters ) );
}

me::IVertexShader::ptr Renderer::ProduceVS( VertexShaderParameters parameters ) 
{
	return me::IVertexShader::ptr( new VertexShader( this, parameters ) );
}

me::IPixelShader::ptr Renderer::ProducePS( PixelShaderParameters parameters ) 
{
	return me::IPixelShader::ptr( new PixelShader( this, parameters ) );
}

me::ITexture::ptr Renderer::ProduceT( TextureParameters parameters ) 
{
	return me::ITexture::ptr( new Texture( this, parameters ) );
}

me::IVertexConstruct::ptr Renderer::ProduceVC( const me::VertexDeclaration & vd, const me::IVertexShader & vs ) 
{
	return me::IVertexConstruct::ptr( new VertexConstruct( this, vd, vs ) );
}
