// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/win/DXRenderer.h>
#include <dxi/win/WindowsOS.h>
#include <dxi/core/Game.h>
#include <dxi/exception/NotImplemented.h>
#include <cassert>

using namespace dxi;
using namespace win;

DXRenderer::DXRenderer( WindowsOS * os )
: m_OS( os )
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
#ifdef DX11
	unsigned int numberOfViewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE; // Can crash if this is above this value.
	D3D11_VIEWPORT * viewports = 0;
	win::DX::GetDxContext()->RSGetViewports( &numberOfViewports, viewports );
	return numberOfViewports;
#else
	return 1;
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
#ifdef DX11
	unsigned int realNumberOfViewports = GetNumberOfViewports();
	D3D11_VIEWPORT dxViewports[ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE ];
	win::DX::GetDxContext()->RSGetViewports( &realNumberOfViewports, dxViewports );
	for ( size_t v = 0; v < numberOfViewports && v < realNumberOfViewports; ++v )
	{
		viewports[ v ].SetTopLeftX( dxViewports[ v ].TopLeftX );
		viewports[ v ].SetTopLeftY( dxViewports[ v ].TopLeftY );
		viewports[ v ].SetWidth( dxViewports[ v ].Width );
		viewports[ v ].SetHeight( dxViewports[ v ].Height );
		viewports[ v ].SetMinDepth( dxViewports[ v ].MinDepth );
		viewports[ v ].SetMaxDepth( dxViewports[ v ].MaxDepth );
	}
	numberOfViewports = realNumberOfViewports;
#else
	throw new exception::NotImplemented( "DXRenderer::GetViewport" );
#endif
}

void DXRenderer::SetViewports( size_t & numberOfViewports, const Viewport * viewports )
{
#ifdef DX11
	D3D11_VIEWPORT dxViewports[ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE ];
	for ( size_t v = 0; v < numberOfViewports && v < numberOfViewports; ++v )
	{
		dxViewports[ v ].TopLeftX = viewports[ 0 ].GetTopLeftX();
		dxViewports[ v ].TopLeftY = viewports[ 0 ].GetTopLeftY();
		dxViewports[ v ].Width = viewports[ 0 ].GetWidth();
		dxViewports[ v ].Height = viewports[ 0 ].GetHeight();
		dxViewports[ v ].MinDepth = viewports[ 0 ].GetMinDepth();
		dxViewports[ v ].MaxDepth = viewports[ 0 ].GetMaxDepth();
	}
	win::DX::GetDxContext()->RSSetViewports( numberOfViewports, dxViewports );
#else
	throw new exception::NotImplemented( "DXRenderer::SetViewport" );
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
}

void DXRenderer::DestroyDirectX()
{
	m_dxDevice = nullptr;
}

void DXRenderer::BeforeRender()
{
	HRESULT hr;

	hr = GetDxDevice()->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 60 ), 1.0f, 0 );
	if( FAILED( hr ) )
	{
		throw unify::Exception( "Failed to clear in BeforeRender!" );
	}

	hr = GetDxDevice()->BeginScene();
	if( FAILED( hr ) )
	{
		throw unify::Exception( "Failed to BeginScene in BeforeRender!" );
	}
}

void DXRenderer::AfterRender()
{
	HRESULT hr;

	hr = GetDxDevice()->EndScene();
	if( FAILED( hr ) )
	{
		throw unify::Exception( "Failed to EndScene in AfterRender!" );
	}

	hr = GetDxDevice()->Present( 0, 0, 0, 0 );
	if( FAILED( hr ) )
	{
		throw unify::Exception( "Failed to Present in AfterRender!" );
	}
}

void DXRenderer::SetCullMode( CullMode::TYPE mode )
{
#if defined( DIRECTX9 )
	struct CullModeValues
	{
		enum
		{
			None = D3DCULL_NONE,
			Clockwise = D3DCULL_CW,
			CounteClockwise = D3DCULL_CCW
		};
	};

	D3DCULL dxCull{};
	switch( mode )
	{
	case CullMode::None: dxCull = D3DCULL_NONE;
	case CullMode::Clockwise: dxCull = D3DCULL_CW;
	case CullMode::CounteClockwise: dxCull = D3DCULL_CCW;
	};

	GetDxDevice()->SetRenderState( D3DRS_CULLMODE, dxCull );
#elif defined( DIRECTX11 )
	throw exception::NotImplemented( "DX11" );
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
