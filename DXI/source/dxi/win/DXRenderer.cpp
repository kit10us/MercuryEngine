// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/win/DXRenderer.h>
#include <dxi/win/WindowsOS.h>
#include <dxi/win/DXDevice.h>
#include <dxi/core/Game.h>
#include <dxi/exception/FailedToCreate.h>
#include <dxi/exception/NotImplemented.h>
#include <cassert>

using namespace dxi;
using namespace win;

DXRenderer::DXRenderer( WindowsOS * os, core::Display display )
	: m_OS( os )
	, m_display( display )
#if defined( DIRECTX9 )
#elif defined( DIRECTX11 )
	, m_swapChainDesc {}
#endif
{
	m_dxDevice = os->GetDxDevice();

	m_pp = {};
	m_pp.BackBufferWidth = (UINT)display.GetSize().width;
	m_pp.BackBufferHeight = (UINT)display.GetSize().height;
	m_pp.BackBufferFormat = D3DFMT_X8R8G8B8;
	m_pp.BackBufferCount = 1;
	m_pp.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_pp.MultiSampleQuality = 0;
	m_pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_pp.hDeviceWindow = display.GetHandle();
	m_pp.Windowed = display.IsFullscreen() ? 0 : 1;
	m_pp.EnableAutoDepthStencil = true;
	m_pp.AutoDepthStencilFormat = D3DFMT_D16; // D3DFMT_D24S8
	m_pp.Flags = 0;

	if( display.IsFullscreen() )
	{
		m_pp.FullScreen_RefreshRateInHz = 60;
		m_pp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}

	IDirect3D9 * dx = Direct3DCreate9( D3D_SDK_VERSION );
	if( !dx )
	{
		throw std::exception( "Failed to create DX!" );
	}

	bool hardwareAcceleration = true; // TODO: This should be configured elsewhere.
	D3DDEVTYPE deviceType = hardwareAcceleration ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF;
	unsigned int behaviorFlags = 0;
	behaviorFlags |= hardwareAcceleration ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	HRESULT hr = S_OK;

	if( m_dxDevice == 0 )
	{
		hr = dx->CreateDevice( 0, deviceType, display.GetHandle(), behaviorFlags, &m_pp, &m_dxDevice );
		if( FAILED( hr ) )
		{
			// Direct-X attempts to fix the presentation parameters, so a second attempt might resolve.
			hr = dx->CreateDevice( 0, deviceType, display.GetHandle(), behaviorFlags, &m_pp, &m_dxDevice );
			if( FAILED( hr ) )
			{
				throw std::exception( "Failed to create Direct-X device!" );
			}
		}

		IDirect3DSwapChain9 * swapChain{};
		m_dxDevice->GetSwapChain( 0, &swapChain );
		SetSwapChain( swapChain );
		DX::SetDxDevice( m_dxDevice );
	}
	else
	{
		IDirect3DSwapChain9 * swapChain{};
		hr = m_dxDevice->CreateAdditionalSwapChain( &m_pp, &swapChain );
		if( FAILED( hr ) )
		{
			throw std::exception( "Failed to create Direct-X swap chain!" );
		}
		SetSwapChain( swapChain );
	}
	SetDxDevice( m_dxDevice );

	dx->Release();
	dx = 0;

	// Set our view matrix...
	D3DXMATRIX finalMatrix;

	D3DXMatrixOrthoOffCenterLH( &finalMatrix, 0, display.GetSize().width, display.GetSize().height, 0, display.GetNearZ(), display.GetFarZ() );
	m_dxDevice->SetTransform( D3DTS_PROJECTION, &finalMatrix );

	GetDxDevice()->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF );
	GetDxDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	GetDxDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	GetDxDevice()->SetRenderState( D3DRS_COLORVERTEX, 1 );
	GetDxDevice()->SetRenderState( D3DRS_LIGHTING, 0 );
}

DXRenderer::~DXRenderer()
{
}

const core::Display & DXRenderer::GetDisplay() const
{
	return m_display;
}

void DXRenderer::SetPP( D3DPRESENT_PARAMETERS pp )
{
	m_pp = pp;
}

const D3DPRESENT_PARAMETERS & DXRenderer::GetPP() const
{
	return m_pp;
}

void DXRenderer::SetDxDevice( IDirect3DDevice9 * dxDevice )
{
	m_dxDevice = dxDevice;
}

IDirect3DDevice9 * DXRenderer::GetDxDevice()
{
	return m_dxDevice;
}

void DXRenderer::SetSwapChain( IDirect3DSwapChain9 * swapChain )
{
	m_swapChain = swapChain;
}

IDirect3DSwapChain9 * DXRenderer::GetSwapChain()
{
	return m_swapChain;
}
	   
/*
void DXRenderer::CreateDirectX()
{
	bool debug =
#if defined( DEBUG ) || defined( _DEBUG )
		true;
#else
		false;
#endif

	auto && os = *m_OS;

#if defined( DIRECTX9 )
	IDirect3D9 * dx = Direct3DCreate9( D3D_SDK_VERSION );
	if( !dx )
	{
		throw unify::Exception( "Failed to create DX!" );
	}
#endif 

#if defined( DIRECTX9 )

	memset( &m_pp, 0, sizeof( m_pp ) );
	m_pp.Windowed = os.GetFullscreen() ? 0 : 1;
	m_pp.BackBufferWidth = os.GetResolution().width;
	m_pp.BackBufferHeight = os.GetResolution().height;
	m_pp.BackBufferFormat = D3DFMT_X8R8G8B8;
	m_pp.EnableAutoDepthStencil = true;
	m_pp.AutoDepthStencilFormat = D3DFMT_D16; // D3DFMT_D24S8
	m_pp.BackBufferCount = 1;
	m_pp.hDeviceWindow = hWnd;

	if( os.GetFullscreen() )
	{
		m_pp.FullScreen_RefreshRateInHz = 60;
		m_pp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		m_pp.SwapEffect = D3DSWAPEFFECT_COPY;
	}
	else
	{
		m_pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	}

	HRESULT result = dx->CreateDevice( 0, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_pp, &m_dxDevice );
	if( !m_dxDevice )
	{
		throw unify::Exception( "Failed to create Direct-X device!" );
	}

	dx->Release();
	dx = 0;

	DX::SetDxDevice( m_dxDevice );

	// Clear
	GetDxDevice()->Clear( 0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB( 255, 0, 0, 0 ), 1.0f, 0 );
	GetDxDevice()->Present( 0, 0, 0, 0 );

	//TODO: Working on a no render state, shader friendly version
	// Default projection matrix
	D3DXMATRIX projectionMatrix;
	float aspectRatio = (float)os.GetResolution().width / os.GetResolution().height;
	D3DXMatrixPerspectiveFovLH( &projectionMatrix, D3DX_PI / 4.0f, aspectRatio, 1, 1000 );
	GetDxDevice()->SetTransform( D3DTS_PROJECTION, &projectionMatrix );
	GetDxDevice()->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF );
	GetDxDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	GetDxDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	GetDxDevice()->SetRenderState( D3DRS_COLORVERTEX, 1 );
	GetDxDevice()->SetRenderState( D3DRS_LIGHTING, 0 );
#elif defined( DIRECTX11 )
	HRESULT result = S_OK;

	m_swapChainDesc.BufferCount = 1;
	m_swapChainDesc.BufferDesc.Width = os.GetResolution().width;
	m_swapChainDesc.BufferDesc.Height = os.GetResolution().height;
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
	if( FAILED( result ) )
	{
		throw exception::FailedToCreate( "Failed to create Direct-X 11!" );
	}

	// Create the back buffer...

	CComPtr< ID3D11Texture2D > backBuffer;
	result = m_swapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (void**)&backBuffer );
	if( FAILED( result ) )
	{
		m_swapChain = 0;
		m_dxDevice = 0;
		m_dxContext = 0;
		throw exception::FailedToCreate( "Failed to get backbuffer during device creation!" );
	}

	result = m_dxDevice->CreateRenderTargetView( backBuffer, 0, &m_renderTargetView );
	backBuffer = 0;
	if( FAILED( result ) )
	{
		m_swapChain = 0;
		m_dxDevice = 0;
		m_dxContext = 0;
		throw exception::FailedToCreate( "Failed to create render target view during device creation!" );
	}

	D3D11_TEXTURE2D_DESC depthStencilDesc = D3D11_TEXTURE2D_DESC();
	depthStencilDesc.Width = os.GetResolution().width;
	depthStencilDesc.Height = os.GetResolution().height;
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
	if ( FAILED( result ) )
	{
		throw exception::FailedToCreate( "Failed to create depth stencil view during device creation!" );
	}
	assert( !FAILED( result ) );

	m_dxContext->OMSetRenderTargets( 1, &m_renderTargetView.p, m_depthStencilView.p );

	D3D11_VIEWPORT viewport = D3D11_VIEWPORT();
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
		D3D11_RASTERIZER_DESC rasterizerDesc = D3D11_RASTERIZER_DESC();
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
	DX::SetDxDevice( m_dxDevice );
	DX::SetDxContext( m_dxContext );

	m_dxContext->RSSetState( m_rasterizerState );
#endif
}

void DXRenderer::DestroyDirectX()
{
#if defined( DIRECTX9 )
	m_dxDevice = nullptr;
#elif defined( DIRECTX11 )
	m_rasterizerState = nullptr;
	m_swapChain = nullptr;
	m_renderTargetView = nullptr;
	m_depthStencilBuffer = nullptr;
	m_depthStencilView = nullptr;
	m_dxContext = nullptr;
	m_dxDevice = nullptr;
#endif
}
*/

void DXRenderer::BeforeRender()
{
#if defined( DIRECTX9 )
	HRESULT result;
	result = GetDxDevice()->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 60 ), 1.0f, 0 );
	if( FAILED( result ) )
	{
		throw unify::Exception( "Failed to clear in BeforeRender!" );
	}

	IDirect3DSurface9 * backBuffer;
	result = m_swapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer );
	result = m_dxDevice->SetRenderTarget( 0, backBuffer );

	result = GetDxDevice()->BeginScene();
	if( FAILED( result ) )
	{
		throw unify::Exception( "Failed to BeginScene in BeforeRender!" );
	}

#elif defined( DIRECTX11 )	
	float clearColor[] = { 0.1f, 0.0f, 0.2f, 1.0f };
	m_dxContext->ClearRenderTargetView( m_renderTargetView, clearColor );
	m_dxContext->ClearDepthStencilView( m_depthStencilView, D3D11_CLEAR_DEPTH , 1.0f, 0 );

	m_dxContext->OMSetDepthStencilState( 0, 0 );
	float blendFactors[] = { 0, 0, 0, 0 };
	m_dxContext->OMSetBlendState( 0, blendFactors, std::numeric_limits< unsigned int >::max() );
#endif
}

void DXRenderer::AfterRender()
{
#if defined( DIRECTX9 )
	HRESULT result;
	result = GetDxDevice()->EndScene();
	if( FAILED( result ) )
	{
		throw unify::Exception( "Failed to EndScene in AfterRender!" );
	}

	result = m_swapChain->Present( 0, 0, 0, 0, 0 );

	if( FAILED( result ) )
	{
		throw unify::Exception( "Failed to Present in AfterRender!" );
	}
#elif defined( DIRECTX11 )
	m_swapChain->Present( 0, 0 ); 
#endif
}

void DXRenderer::SetCullMode( CullMode::TYPE mode )
{
#if defined( DIRECTX9 )
	D3DCULL dxCull{};
	switch( mode )
	{
	case CullMode::None: dxCull = D3DCULL_NONE; break;
	case CullMode::Clockwise: dxCull = D3DCULL_CW; break;
	case CullMode::CounteClockwise: dxCull = D3DCULL_CCW; break;
	};

	GetDxDevice()->SetRenderState( D3DRS_CULLMODE, dxCull );
#elif defined( DIRECTX11 )
	// TODO: throw exception::NotImplemented( "DX11" );
#endif
}

Viewport DXRenderer::GetViewport() const
{
	return Viewport( 0, 0, GetDisplay().GetSize().width, GetDisplay().GetSize().height, GetDisplay().GetNearZ(), GetDisplay().GetFarZ() );
}

bool DXRenderer::IsFullscreen() const
{
	return m_display.IsFullscreen();
}

HWND DXRenderer::GetHandle() const
{
	return  m_display.GetHandle();
}

