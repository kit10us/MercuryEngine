// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <medx9/Renderer.h>
#include <medx9/VertexBuffer.h>
#include <medx9/IndexBuffer.h>
#include <medx9/VertexShader.h>
#include <medx9/PixelShader.h>
#include <medx9/VertexConstruct.h>
#include <medx9/Texture.h>
#include <me/RenderMethod.h>
#include <me/exception/FailedToCreate.h>
#include <me/exception/NotImplemented.h>									
#include <me/exception/Render.h>
#include <cassert>

using namespace medx9;
using namespace me;

Renderer::Renderer( WindowsOS * os, me::Display display, size_t index )
		: m_os( os )
		, m_display( display )
		, m_index( index )
{
	for ( int i = 0; i < os->RendererCount(); ++i )
	{
		auto Renderer = dynamic_cast< medx9::Renderer *>(os->GetRenderer( i ));
		if ( Renderer && Renderer->GetDxDevice() )
		{
			m_dxDevice = Renderer->GetDxDevice();
		}
	}

	m_pp = {};
	m_pp.BackBufferWidth = (UINT)display.GetSize().width;
	m_pp.BackBufferHeight = (UINT)display.GetSize().height;
	m_pp.BackBufferFormat = D3DFMT_X8R8G8B8;
	m_pp.BackBufferCount = 1;
	m_pp.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_pp.MultiSampleQuality = 0;
	m_pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_pp.hDeviceWindow = (HWND)display.GetHandle();
	m_pp.Windowed = display.IsFullscreen() ? 0 : 1;
	m_pp.EnableAutoDepthStencil = true;
	m_pp.AutoDepthStencilFormat = D3DFMT_D16; // D3DFMT_D24S8
	m_pp.Flags = 0;

	if ( display.IsFullscreen() )
	{
		m_pp.FullScreen_RefreshRateInHz = 60;
		m_pp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}

	IDirect3D9 * dx = Direct3DCreate9( D3D_SDK_VERSION );
	if ( !dx )
	{
		throw std::exception( "Failed to create DX!" );
	}

	bool hardwareAcceleration = true; // TODO: This should be configured elsewhere.
	D3DDEVTYPE deviceType = hardwareAcceleration ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF;
	unsigned int behaviorFlags = 0;
	behaviorFlags |= hardwareAcceleration ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	HRESULT hr = S_OK;

	if ( m_dxDevice == 0 )
	{
		hr = dx->CreateDevice( 0, deviceType, (HWND)display.GetHandle(), behaviorFlags, &m_pp, &m_dxDevice );
		if ( FAILED( hr ) )
		{
			// Direct-X attempts to fix the presentation parameters, so a second attempt might resolve.
			hr = dx->CreateDevice( 0, deviceType, (HWND)display.GetHandle(), behaviorFlags, &m_pp, &m_dxDevice );
			if ( FAILED( hr ) )
			{
				throw std::exception( "Failed to create Direct-X device!" );
			}
		}

		m_dxDevice->GetSwapChain( 0, &m_swapChain );
	}
	else
	{
		IDirect3DSwapChain9 * swapChain{};
		hr = m_dxDevice->CreateAdditionalSwapChain( &m_pp, &m_swapChain );
		if ( FAILED( hr ) )
		{
			throw std::exception( "Failed to create Direct-X swap chain!" );
		}
	}

	dx->Release();
	dx = 0;

	// Set our view matrix...
	D3DXMATRIX finalMatrix;

	D3DXMatrixOrthoOffCenterLH( &finalMatrix, 0, display.GetSize().width, display.GetSize().height, 0, display.GetNearZ(), display.GetFarZ() );
	m_dxDevice->SetTransform( D3DTS_PROJECTION, &finalMatrix );

	m_dxDevice->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF );
	m_dxDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	m_dxDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	m_dxDevice->SetRenderState( D3DRS_COLORVERTEX, 1 );
	m_dxDevice->SetRenderState( D3DRS_LIGHTING, 0 );
}

Renderer::~Renderer()
{
}

me::IGame * Renderer::GetGame()
{
	return m_os->GetGame();
}

const me::Display & Renderer::GetDisplay() const
{
	return m_display;
}

IDirect3DDevice9 * Renderer::GetDxDevice() const
{
	return m_dxDevice;
}
	   
void Renderer::BeforeRender()
{
	HRESULT result;
	result = m_dxDevice->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 90 ), 1.0f, 0 );
	if ( FAILED( result ) )
	{
		throw unify::Exception( "Failed to clear in BeforeRender!" );
	}

	IDirect3DSurface9 * backBuffer;
	result = m_swapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer );
	result = m_dxDevice->SetRenderTarget( 0, backBuffer );

	result = m_dxDevice->BeginScene();
	if ( FAILED( result ) )
	{
		throw unify::Exception( "Failed to BeginScene in BeforeRender!" );
	}
}

void Renderer::AfterRender()
{
	HRESULT result;
	result = m_dxDevice->EndScene();
	if ( FAILED( result ) )
	{
		throw unify::Exception( "Failed to EndScene in AfterRender!" );
	}

	result = m_swapChain->Present( 0, 0, 0, 0, 0 );

	if ( FAILED( result ) )
	{
		throw unify::Exception( "Failed to Present in AfterRender!" );
	}
}

void Renderer::SetCullMode( me::CullMode::TYPE mode )
{
	D3DCULL dxCull{};
	switch ( mode )
	{
	case CullMode::None: dxCull = D3DCULL_NONE; break;
	case CullMode::Clockwise: dxCull = D3DCULL_CW; break;
	case CullMode::CounteClockwise: dxCull = D3DCULL_CCW; break;
	};

	m_dxDevice->SetRenderState( D3DRS_CULLMODE, dxCull );
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

void Renderer::Render( const RenderMethod & method, const RenderInfo & renderInfo, MatrixFeed & matrixFeed )
{
	while( ! matrixFeed.Done() )
	{
		unify::Matrix matrix;
		matrixFeed.Consume( &matrix, 1 );

		D3DPRIMITIVETYPE dxPrimitiveType {};
		switch( method.primitiveType )
		{
		case PrimitiveType::PointList: dxPrimitiveType = D3DPT_POINTLIST; break;
		case PrimitiveType::LineList: dxPrimitiveType = D3DPT_LINELIST; break;
		case PrimitiveType::LineStrip: dxPrimitiveType = D3DPT_LINESTRIP; break;
		case PrimitiveType::TriangleList: dxPrimitiveType = D3DPT_TRIANGLELIST;	break;
		case PrimitiveType::TriangleStrip: dxPrimitiveType = D3DPT_TRIANGLESTRIP;  break;
		}
							   
		auto dxDevice = GetDxDevice();
		
		HRESULT hr = S_OK;

		if ( method.effect )
		{
			method.effect->UpdateData( renderInfo, &matrix, 1 );
			method.effect->Use();
		}

		// Draw Primitive...
		if( method.useIB == false )
		{
			hr = dxDevice->DrawPrimitive( dxPrimitiveType, method.startVertex, method.primitiveCount );
		}
		else
		{
			hr = dxDevice->DrawIndexedPrimitive( dxPrimitiveType, method.baseVertexIndex, method.minIndex, method.vertexCount, method.startIndex, method.primitiveCount );
		}
		if ( FAILED(hr) )
		{
			throw exception::Render( "Failed to render vertex buffer!" );
		}
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

me::IVertexConstruct::ptr Renderer::ProduceVC( const VertexDeclaration & vd, const IVertexShader & vs )
{
	return me::IVertexConstruct::ptr( new VertexConstruct( this, vd ) );
}

me::ITexture::ptr Renderer::ProduceT( TextureParameters parameters )
{
	return me::ITexture::ptr( new Texture( this, parameters ) );
}
