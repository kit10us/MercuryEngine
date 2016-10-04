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

WindowsOS::WindowsOS()
: m_defaultViewport( 0, 0, 800.0f, 600.0f, 0, 1 )
, m_fullscreen{}
, m_hasFocus{}
, m_renderer{}
, m_hInstance{}
, m_nCmdShow{}
, m_hWnd{}
{
}

WindowsOS::WindowsOS( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow )
: WindowsOS()
{
	{
		using namespace std;
		char buffer[MAX_PATH];
		GetModuleFileNameA( NULL, buffer, MAX_PATH );
		string::size_type pos = string( buffer ).find_last_of( "\\/" );
		if( pos != string::npos ) 
		{
			m_name = buffer;
		}
	}
		
	m_hInstance = hInstance;
	m_nCmdShow = nCmdShow;
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

WindowsOS::WindowsOS( HWND hWnd )
: WindowsOS()
{
	m_hWnd = hWnd;
	m_hInstance = (HINSTANCE)GetWindowLong( m_hWnd, GWL_HINSTANCE );
}

WindowsOS::~WindowsOS()
{
	m_renderer.reset();
}

std::string WindowsOS::GetName() const
{
	return m_name;
}

const std::vector< std::string > & WindowsOS::GetCommandLine() const
{
	return m_commandLine;
}

core::IRenderer * WindowsOS::GetRenderer() const
{
	return m_renderer.get();
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

void WindowsOS::Startup()
{
	if ( ! m_hWnd )
	{
		CreateWindow( m_hWnd );
	}

	DragAcceptFiles( this->GetHWnd(), true );
	CreateDirectX();
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
	m_renderer.reset( new DXRenderer( this ) );
}

void WindowsOS::DestroyDirectX()
{
	m_renderer.reset();
}
