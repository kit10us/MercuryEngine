// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/win/WindowsOS.h>
#include <dxi/win/DxDevice.h>
#include <unify/Exception.h>
#include <shellapi.h>

using namespace dxi;
using namespace win;

#ifdef GetCommandLine
#undef GetCommandLine
#endif

extern "C" LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

WindowsOS::WindowsOS( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow )
: m_hInstance( hInstance )
, m_nCmdShow( nCmdShow )
, m_defaultViewport( 0, 0, 800.0f, 600.0f, 0, 1 )
, m_fullscreen( false )
, m_hasFocus( false )
, m_hWnd( 0 )
, m_renderer( 0 )
{
	hPrevInstance; // NOT USED

	// Parse the commandline...
	std::string commandLineString( lpszCmdLine );
	size_t l = 0;
	size_t r = 0;
	bool inQuote = false;
	std::string working;

	for( size_t l = 0, r = 0; r <= commandLineString.length(); ++r )
	{
		if ( ! inQuote && ( r == commandLineString.length()  || commandLineString.at( r ) == ' ' ) )
		{
			if ( l != r )
			{
				working += commandLineString.substr( l, r - l );
			}
			if ( working.empty() == false )
			{
				m_commandLine.push_back( working );
				working.clear();
			}
			l = r + 1;
		}
		else if ( commandLineString.at( r ) == '\"' )
		{
			// Include partial string...
			working += commandLineString.substr( l, r - l );
			l = r + 1; // One past the double quote.
			inQuote = !inQuote;
		}
	}
}

WindowsOS::~WindowsOS()
{
	DestroyDirectX();
}

const std::vector< std::string > & WindowsOS::GetCommandLine() const
{
	return m_commandLine;
}

core::IRenderer * WindowsOS::GetRenderer() const
{
	return m_renderer;
}

void WindowsOS::SetResolution( const unify::Size< unsigned int > & resolution )
{
	m_defaultViewport.SetWidth( static_cast< float >( resolution.width ) );
	m_defaultViewport.SetHeight( static_cast< float >( resolution.height ) );
}

void WindowsOS::SetFullscreen( bool fullscreen )
{
	m_fullscreen = fullscreen;
}

unify::Size< unsigned int > WindowsOS::GetResolution() const
{
	return unify::Size< unsigned int >( static_cast< unsigned int >( m_defaultViewport.GetWidth() ), static_cast< unsigned int >( m_defaultViewport.GetHeight() ) );
}

const Viewport & WindowsOS::GetDefaultViewport() const
{
	return m_defaultViewport;
}

bool WindowsOS::GetFullscreen() const
{
	return m_fullscreen;
}
			
void WindowsOS::SetHasFocus( bool hasFocus )
{
	m_hasFocus = hasFocus;
}

bool WindowsOS::GetHasFocus() const
{
	return m_hasFocus;
}

HINSTANCE WindowsOS::GetHInstance()
{
	return m_hInstance;
}

HWND WindowsOS::GetHWnd()
{
	return m_hWnd;
}

IDirect3DDevice9 * WindowsOS::GetDxDevice()
{
	return m_dxDevice;
}

void WindowsOS::Startup()
{
	CreateWindow( m_hWnd );
	DragAcceptFiles( this->GetHWnd(), true );
	CreateDirectX();
	DX::SetDxDevice( m_dxDevice );
}

void WindowsOS::BeforeRender()
{
	HRESULT hr;

	hr = GetDxDevice()->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,60), 1.0f, 0 );
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

void WindowsOS::AfterRender()
{
	HRESULT hr;
	
	hr = GetDxDevice()->EndScene();
	if( FAILED(hr) )
	{
		throw unify::Exception( "Failed to EndScene in AfterRender!" );
	}

	hr = GetDxDevice()->Present( 0, 0, 0, 0 );
	if( FAILED(hr) )
	{
		throw unify::Exception( "Failed to Present in AfterRender!" );
	}
}

void WindowsOS::Shutdown()
{
	DragAcceptFiles( this->GetHWnd(), false );
	DX::SetDxDevice( 0 );
	DestroyDirectX();
}

void WindowsOS::DebugWrite( const std::string & text )
{
	OutputDebugStringA( text.c_str() );
}

void WindowsOS::DebugWriteLine( const std::string & line )
{
	DebugWrite( line + "\n" );
}


void WindowsOS::CreateWindow( HWND & hwnd )
{
	WNDCLASS wc;
	memset( &wc, 0, sizeof( wc ) );
	wc.style = 0;
    wc.lpfnWndProc = (WNDPROC) WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hInstance;
    wc.hIcon = LoadIcon( (HINSTANCE) NULL,IDI_APPLICATION );
    wc.hCursor = LoadCursor( (HINSTANCE) NULL, IDC_ARROW );
    //wc.hbrBackground = GetStockObject(WHITE_BRUSH); 
    wc.lpszMenuName =  L"MainMenu"; 
    wc.lpszClassName = L"MainWndClass"; 
 
    if ( !RegisterClass( &wc ) )
	{
		throw unify::Exception( "Failed to register window class!" );
    } 

    hwnd = CreateWindowW( 
		L"MainWndClass", L"Sample", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		GetResolution().width, GetResolution().height, (HWND) 0, (HMENU) 0, m_hInstance, (LPVOID) 0); 
 
    if ( !hwnd )
	{
		throw unify::Exception( "Failed to create window!" );
	}

    ShowWindow( hwnd, m_nCmdShow ); 
    UpdateWindow( hwnd ); 
}

void WindowsOS::CreateDirectX()
{
	bool debug =
#if defined( DEBUG ) || defined( _DEBUG )
		true;
#else
		false;
#endif

	IDirect3D9 * dx = Direct3DCreate9( D3D_SDK_VERSION );
	if ( !dx )
	{
		throw unify::Exception( "Failed to create DX!" );
	}

	memset( &m_pp, 0, sizeof( m_pp ) );
	m_pp.Windowed = GetFullscreen() ? 0 : 1;
	m_pp.BackBufferWidth = GetResolution().width;
	m_pp.BackBufferHeight = GetResolution().height;
    m_pp.BackBufferFormat = D3DFMT_X8R8G8B8;
    m_pp.EnableAutoDepthStencil = true;
    m_pp.AutoDepthStencilFormat = D3DFMT_D16; // D3DFMT_D24S8
    m_pp.BackBufferCount = 1;
	m_pp.hDeviceWindow = m_hWnd;
	
    if( GetFullscreen() )
    {
	    m_pp.FullScreen_RefreshRateInHz = 60;
		m_pp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		m_pp.SwapEffect = D3DSWAPEFFECT_COPY;
    }
    else
    {
        m_pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    }

	HRESULT result = dx->CreateDevice( 0, D3DDEVTYPE_HAL, m_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_pp, &m_dxDevice );
	if ( ! m_dxDevice )
	{
		throw unify::Exception( "Failed to create Direct-X device!" );
	}

	dx->Release();
	dx = 0;

	DX::SetDxDevice( m_dxDevice );
	m_renderer = new DXRenderer( this );

	// Clear
	GetDxDevice()->Clear( 0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB( 255, 0, 0, 0 ), 1.0f, 0 );
	GetDxDevice()->Present( 0, 0, 0, 0 );

	//TODO: Working on a no render state, shader friendly version
	// Default projection matrix
	D3DXMATRIX projectionMatrix;
	float aspectRatio = (float)GetResolution().width / GetResolution().height;
	D3DXMatrixPerspectiveFovLH( &projectionMatrix, D3DX_PI / 4.0f, aspectRatio, 1, 1000 );
	GetDxDevice()->SetTransform( D3DTS_PROJECTION, &projectionMatrix );
	GetDxDevice()->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF );
	GetDxDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	GetDxDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	GetDxDevice()->SetRenderState( D3DRS_COLORVERTEX, 1 );
	GetDxDevice()->SetRenderState( D3DRS_LIGHTING, 0 );
}

void WindowsOS::DestroyDirectX()
{
	if ( m_renderer )
	{
		delete m_renderer;
		m_renderer = 0;
	}

	m_dxDevice = 0;
}
