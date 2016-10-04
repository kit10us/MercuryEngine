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

DXRenderer::DXRenderer( WindowsOS * os )
	: m_OS( os )
#if defined( DIRECTX9 )
#elif defined( DIRECTX11 )
	, m_swapChainDesc {}
#endif
{
	CreateDirectX();
}

DXRenderer::~DXRenderer()
{
	DestroyDirectX();
	m_OS = 0;
}

size_t DXRenderer::GetNumberOfViewports() const
{
#if defined( DIRECTX9 )
	return 1;
#elif defined( DIRECTX11 )
	unsigned int numberOfViewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE; // Can crash if this is above this value.
	D3D11_VIEWPORT * viewports = 0;
	win::DX::GetDxContext()->RSGetViewports( &numberOfViewports, viewports );
	return numberOfViewports;
#endif
}

void DXRenderer::GetViewport( Viewport & viewport )
{
#if defined( DIRECTX9 )
	D3DVIEWPORT9 dxViewport = {};
	win::DX::GetDxDevice()->GetViewport( &dxViewport );
	viewport.SetTopLeftX( (float)dxViewport.X );
	viewport.SetTopLeftY( (float)dxViewport.Y );
	viewport.SetWidth( (float)dxViewport.Width );
	viewport.SetHeight( (float)dxViewport.Height );
	viewport.SetMinDepth( dxViewport.MinZ );
	viewport.SetMaxDepth( dxViewport.MaxZ );
#elif defined( DIRECTX11 )
	unsigned int numberOfViewports = 1;
	D3D11_VIEWPORT dxViewport[1];
	win::DX::GetDxContext()->RSGetViewports( &numberOfViewports, dxViewport );
	viewport.SetTopLeftX( dxViewport[0].TopLeftX );
	viewport.SetTopLeftY( dxViewport[0].TopLeftY );
	viewport.SetWidth( dxViewport[0].Width );
	viewport.SetHeight( dxViewport[0].Height );
	viewport.SetMinDepth( dxViewport[0].MinDepth );
	viewport.SetMaxDepth( dxViewport[0].MaxDepth );
#endif
}

void DXRenderer::SetViewport( const Viewport & viewport )
{
#if defined( DIRECTX9 )
	D3DVIEWPORT9 dxViewport;
	dxViewport.X = (DWORD)viewport.GetTopLeftX();
	dxViewport.Y = (DWORD)viewport.GetTopLeftY();
	dxViewport.Width = (DWORD)viewport.GetWidth();
	dxViewport.Height = (DWORD)viewport.GetHeight();
	dxViewport.MinZ = viewport.GetMinDepth();
	dxViewport.MaxZ = viewport.GetMaxDepth();
	win::DX::GetDxDevice()->SetViewport( &dxViewport );
#elif defined( DIRECTX11 )
	unsigned int numberOfViewports = 1;
	D3D11_VIEWPORT dxViewport;
	dxViewport.TopLeftX = viewport.GetTopLeftX();
	dxViewport.TopLeftY = viewport.GetTopLeftY();
	dxViewport.Width = viewport.GetWidth();
	dxViewport.Height = viewport.GetHeight();
	dxViewport.MinDepth = viewport.GetMinDepth();
	dxViewport.MaxDepth = viewport.GetMaxDepth();
	win::DX::GetDxContext()->RSSetViewports( numberOfViewports, &dxViewport );
#endif
}

void DXRenderer::GetViewports( size_t & numberOfViewports, Viewport * viewports )
{
#if defined( DIRECTX9 )
	throw new exception::NotImplemented( "DXRenderer::GetViewport" );
#elif defined( DIRECTX11 )
	unsigned int realNumberOfViewports = GetNumberOfViewports();
	D3D11_VIEWPORT dxViewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	win::DX::GetDxContext()->RSGetViewports( &realNumberOfViewports, dxViewports );
	for( size_t v = 0; v < numberOfViewports && v < realNumberOfViewports; ++v )
	{
		viewports[v].SetTopLeftX( dxViewports[v].TopLeftX );
		viewports[v].SetTopLeftY( dxViewports[v].TopLeftY );
		viewports[v].SetWidth( dxViewports[v].Width );
		viewports[v].SetHeight( dxViewports[v].Height );
		viewports[v].SetMinDepth( dxViewports[v].MinDepth );
		viewports[v].SetMaxDepth( dxViewports[v].MaxDepth );
	}
	numberOfViewports = realNumberOfViewports;
#endif
}

void DXRenderer::SetViewports( size_t & numberOfViewports, const Viewport * viewports )
{
#if defined( DIRECTX9 )
	throw new exception::NotImplemented( "DXRenderer::SetViewport" );
#elif defined( DIRECTX11 )
	D3D11_VIEWPORT dxViewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	for( size_t v = 0; v < numberOfViewports && v < numberOfViewports; ++v )
	{
		dxViewports[v].TopLeftX = viewports[0].GetTopLeftX();
		dxViewports[v].TopLeftY = viewports[0].GetTopLeftY();
		dxViewports[v].Width = viewports[0].GetWidth();
		dxViewports[v].Height = viewports[0].GetHeight();
		dxViewports[v].MinDepth = viewports[0].GetMinDepth();
		dxViewports[v].MaxDepth = viewports[0].GetMaxDepth();
	}
	win::DX::GetDxContext()->RSSetViewports( numberOfViewports, dxViewports );
#endif
}

void DXRenderer::CreateDirectX()
{
	bool debug =
#if defined( DEBUG ) || defined( _DEBUG )
		true;
#else
		false;
#endif

	auto && os = *m_OS;
	HWND hWnd = os.GetHWnd();  // TODO: Will this support dual output?

#if defined( DIRECTX9 )
	IDirect3D9 * dx = Direct3DCreate9( D3D_SDK_VERSION );
	if( !dx )
	{
		throw unify::Exception( "Failed to create DX!" );
	}

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

void DXRenderer::BeforeRender()
{
#if defined( DIRECTX9 )
	HRESULT result;
	result = GetDxDevice()->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 60 ), 1.0f, 0 );
	if( FAILED( result ) )
	{
		throw unify::Exception( "Failed to clear in BeforeRender!" );
	}

	result = GetDxDevice()->BeginScene();
	if( FAILED( result ) )
	{
		throw unify::Exception( "Failed to BeginScene in BeforeRender!" );
	}
#elif defined( DIRECTX11 )	
	float clearColor[] = { 0.1f, 0.0f, 0.2f, 1.0f };
	m_dxContext->ClearRenderTargetView( m_renderTargetView, clearColor );
	m_dxContext->ClearDepthStencilView( m_depthStencilView, D3D11_CLEAR_DEPTH /*| D3D11_CLEAR_STENCIL*/, 1.0f, 0 );

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

	result = GetDxDevice()->Present( 0, 0, 0, 0 );
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

#if defined( DIRECTX9 )
IDirect3DDevice9 * DXRenderer::GetDxDevice()
{
	return m_dxDevice;
}
#elif defined( DIRECTX11 )
// TODO:
#endif


#if defined( DIRECTX9 )
#elif defined( DIRECTX11 )
IDXGISwapChain * DXRenderer::GetSwapChain()
{
	//m_swapChain
	return m_swapChain;
}
#endif
