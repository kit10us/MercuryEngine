// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/win/WindowsOS.h>
#include <unify/Exception.h>
#include <unify/Path.h>
#include <dxi/exception/FailedToCreate.h>
#include <shellapi.h>

using namespace dxi;
using namespace win;

#ifdef GetCommandLine
#undef GetCommandLine
#endif

WindowsOS::WindowsOS( core::IGame * game )
: m_game( game )
, m_handle{}
, m_hasFocus{}
, m_hInstance{}
, m_cmdShow{}
, m_wndProc{}
, m_keyboard{}
, m_mouse{}
{
}

WindowsOS::WindowsOS( core::IGame * game, HWND handle )
: WindowsOS( game )
{
	m_handle = handle;
	m_hInstance = (HINSTANCE)GetWindowLong( handle, GWL_HINSTANCE );
}

WindowsOS::WindowsOS( core::IGame * game, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int cmdShow, WNDPROC wndProc )
: WindowsOS( game )
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
	m_cmdShow = cmdShow;
	hPrevInstance; // NOT USED
	m_wndProc = wndProc;

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
	m_renderers.clear();
}

std::string WindowsOS::GetName() const
{
	return m_name;
}

const std::vector< std::string > & WindowsOS::GetCommandLine() const
{
	return m_commandLine;
}

void WindowsOS::AddDisplay( core::Display display )
{
	m_pendingDisplays.push_back( display );
}

void WindowsOS::CreatePendingDisplays()
{
	if( m_pendingDisplays.empty() )
	{
		return;
	}

	for( auto && display : m_pendingDisplays )
	{
		CreateDisplay( display );
	}

	m_pendingDisplays.clear();
}

void WindowsOS::CreateDisplay( core::Display display )
{
	bool isPrimary = m_renderers.empty() ? true : false; // Note that this is VERY explicit - we are actual spelling out our intention (even though it looks redundant).

	std::shared_ptr< DXRenderer > renderer;

	if( display.IsDialog() )
	{
		// Note: If we are a dialog, we do NOT have a Direct-X Display Device.
		HINSTANCE hInstance = GetHInstance(); // [optional] A handle to the module whose executable file contains the dialog box template. 
		LPCSTR lpName = display.GetDialogTemplateName();

		//MAKEINTRESOURCEA( IDD_ENUM ); // The dialog box template. This parameter is either the pointer to a null-terminated character string that specifies the name of the dialog box template or an integer value that specifies the resource identifier of the dialog box template. If the parameter specifies a resource identifier, its high-order word must be zero and its low-order word must contain the identifier. You can use the MAKEINTRESOURCE macro to create this value. 
		HWND hWndParent = display.GetParentHandle(); // [optional] A handle to the window that owns the dialog box. 
		DLGPROC lpDialogFunc = display.GetDialogProc() ? display.GetDialogProc() : (DLGPROC)m_wndProc; // [optional] A pointer to the dialog box procedure. For more information about the dialog box procedure, see DialogProc.
		HWND hwnd = CreateDialogA( hInstance, lpName, hWndParent, lpDialogFunc );
		assert( hwnd );
		if( hWndParent )
		{
			m_childHandles.push_back( hwnd );
		}
		ShowWindow( hwnd, SW_SHOW );
	}
	else
	{
		// If we have no handle, create a window...
		if ( display.GetHandle() == 0 )
		{
			// Regardless of windowed or not, we need a window...
			WNDCLASS wc;
			memset( &wc, 0, sizeof( wc ) );
			wc.style = 0;
			wc.lpfnWndProc = (WNDPROC)m_wndProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = GetHInstance();
			wc.hIcon = LoadIcon( (HINSTANCE)NULL, IDI_APPLICATION );
			wc.hCursor = LoadCursor( (HINSTANCE)NULL, IDC_ARROW );

			if( isPrimary )
			{
				wc.lpszMenuName = L"MainMenu";
				wc.lpszClassName = L"MainWndClass";
			}
			else		    // TODO: This worked, yet is dodgy...
			{
				wc.lpszMenuName = 0;
				wc.lpszClassName = L"SecondWndClass";
			}

			if( !RegisterClass( &wc ) )
			{
				throw std::exception( "Failed to register window class!" );
			}

			int x = static_cast< int >(display.GetPosition().x);
			int y = static_cast< int >(display.GetPosition().y);
			int width = static_cast< int >(display.GetSize().width);
			int height = static_cast< int >(display.GetSize().height);
			HWND parentHandle = display.GetParentHandle();
			HWND handle = CreateWindowA( "MainWndClass", "An Empty DirectX Project", WS_OVERLAPPEDWINDOW, x, y, width, height,
				parentHandle, (HMENU)NULL, GetHInstance(), (LPVOID)NULL );

			if( !handle )
			{
				throw exception::FailedToCreate( "Failed to create window!" );
			}

			{ // Resize window to ensure exact pixel match...
				RECT windowRect;
				GetWindowRect( handle, &windowRect );

				RECT clientRect;
				GetClientRect( handle, &clientRect );

				long newWindowWidth = (windowRect.right - windowRect.left) + width - clientRect.right;
				long newWindowHeight = (windowRect.bottom - windowRect.top) + height - clientRect.bottom;
				MoveWindow( handle, windowRect.left, windowRect.top, newWindowWidth, newWindowHeight, false );
			}

			ShowWindow( handle, m_cmdShow );
			UpdateWindow( handle );
			display.SetHandle( handle );
		}
	}

	renderer.reset( new DXRenderer( this, display ) );
	m_renderers.push_back( renderer );
	if ( renderer->GetDxDevice() )
	{
		m_dxDevice = renderer->GetDxDevice();
	}
}

int WindowsOS::RendererCount() const
{
	return m_renderers.size();
}

core::IRenderer * WindowsOS::GetRenderer( int index) const
{
	return m_renderers[ index ].get();
}

void WindowsOS::SetHasFocus( bool hasFocus )
{
	m_hasFocus = hasFocus;
}

bool WindowsOS::GetHasFocus() const
{
	return m_hasFocus;
}

HINSTANCE WindowsOS::GetHInstance() const
{
	return m_hInstance;
}

HWND WindowsOS::GetHandle() const
{
	if ( m_handle )
	{
		return m_handle;
	}
	else
	{
		return m_renderers[0]->GetDisplay().GetHandle();
	}
}

void WindowsOS::BuildRenderers()
{		 
	CreatePendingDisplays();

	// TODO: DragAcceptFiles( this->GetHWnd(), true );
}

void WindowsOS::Startup()
{
	auto keyboardItr = m_game->GetInputManager()->Find( "Keyboard" );
	if ( keyboardItr )
	{
		m_keyboard = keyboardItr.get();
	}

	auto mouseItr = m_game->GetInputManager()->Find( "Mouse" );
	if ( mouseItr )
	{
		m_mouse = mouseItr.get();
	}
}

void WindowsOS::Shutdown()
{
}

void WindowsOS::DebugWrite( const std::string & text )
{
	OutputDebugStringA( text.c_str() );
}

void WindowsOS::DebugWriteLine( const std::string & line )
{
	DebugWrite( line + "\n" );
}

IDirect3DDevice9 * WindowsOS::GetDxDevice()
{
	return m_dxDevice;
}

LRESULT WindowsOS::WndProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	core::IGame & game = *m_game;
	static bool trackingMouse = false;

	switch ( message )
	{
	case WM_CLOSE: // Fall through to WM_DESTROY...
	case WM_DESTROY:
		game.Quit();
		return 0;

	case WM_MOUSELEAVE:
	{
		if ( m_mouse == nullptr ) break;

		for( int renderer = 0; renderer < RendererCount(); ++renderer )
		{
			if ( GetRenderer( renderer )->GetHandle() == handle )
			{
				trackingMouse = false;
				m_mouse->SetState( renderer, "MouseAvailable", "Available", false );
				break;
			}
		}
	}
	break;

	case WM_LBUTTONDOWN:
	{
		if ( m_mouse == nullptr ) break;

		for ( int renderer = 0; renderer < RendererCount(); ++renderer )
		{
			if ( GetRenderer( renderer )->GetHandle() == handle )
			{
				trackingMouse = false;
				m_mouse->SetState( renderer, "LeftButton", "Down", true );
				break;
			}
		}
	}
	break;

	case WM_LBUTTONUP:
	{
		if ( m_mouse == nullptr ) break;

		for ( int renderer = 0; renderer < RendererCount(); ++renderer )
		{
			if ( GetRenderer( renderer )->GetHandle() == handle )
			{
				trackingMouse = false;
				m_mouse->SetState( renderer, "LeftButton", "Down", false );
				break;
			}
		}
	}
	break;

	case WM_RBUTTONDOWN:
	{
		if ( m_mouse == nullptr ) break;

		for ( int renderer = 0; renderer < RendererCount(); ++renderer )
		{
			if ( GetRenderer( renderer )->GetHandle() == handle )
			{
				trackingMouse = false;
				m_mouse->SetState( renderer, "RightButton", "Down", true );
				break;
			}
		}
	}
	break;

	case WM_RBUTTONUP:
	{
		if ( m_mouse == nullptr ) break;

		for ( int renderer = 0; renderer < RendererCount(); ++renderer )
		{
			if ( GetRenderer( renderer )->GetHandle() == handle )
			{
				trackingMouse = false;
				m_mouse->SetState( renderer, "RightButton", "Down", false );
				break;
			}
		}
	}
	break;

	case WM_MOUSEWHEEL:
	{
		if ( m_mouse == nullptr ) break;

		short zDelta = GET_WHEEL_DELTA_WPARAM( wParam );
		for ( int renderer = 0; renderer < RendererCount(); ++renderer )
		{
			if ( GetRenderer( renderer )->GetHandle() == handle )
			{
				trackingMouse = false;
				m_mouse->SetValue( renderer, "MouseWheel", zDelta / (float)WHEEL_DELTA );
				break;
			}
		}
	}
	break;

	case WM_MOUSEMOVE:
	{
		if ( m_mouse == nullptr ) break;

		// TODO:
		// Enable tracking when the mouse leaves the client area...
		if ( !trackingMouse )
		{
			TRACKMOUSEEVENT trackMouseEvent = TRACKMOUSEEVENT();
			trackMouseEvent.cbSize = sizeof TRACKMOUSEEVENT;
			trackMouseEvent.dwFlags = TME_LEAVE;
			trackMouseEvent.hwndTrack = handle;
			trackMouseEvent.dwHoverTime = HOVER_DEFAULT;
			TrackMouseEvent( &trackMouseEvent );
		}


		RECT clientRect;
		GetClientRect( handle, &clientRect );

		for ( int renderer = 0; renderer < RendererCount(); ++renderer )
		{
			if ( GetRenderer( renderer )->GetHandle() == handle )
			{
				trackingMouse = false;				
								
				float width = static_cast<float>(GetRenderer( renderer )->GetViewport().GetWidth());
				float height = static_cast< float >(GetRenderer( renderer )->GetViewport().GetHeight());
				float clientWidth = static_cast< float >(clientRect.right);
				float clientHeight = static_cast< float >(clientRect.bottom);

				unify::V2< int > mousePosition( static_cast< int >(LOWORD( lParam )), static_cast< int >(HIWORD( lParam )) );
				mousePosition.x *= static_cast< int >(width / clientWidth);
				mousePosition.y *= static_cast< int >(height / clientHeight);

				m_mouse->SetValue( renderer, "PositionX", (float)mousePosition.x );
				m_mouse->SetValue( renderer, "PositionY", (float)mousePosition.y );
				break;
			}
		}
	}
	break;

	case WM_ACTIVATE:
	{
		WORD lowOrder = wParam & 0x0000FFFF;
		bool minimized = (wParam & 0xFFFF0000) != 0;
		switch ( lowOrder )
		{
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
			SetHasFocus( true );
			break;
		case WA_INACTIVE:
			SetHasFocus( false );
			break;

		default:
			assert( 0 && "Invalid activity state!" );
		}
	}
	break;
	/*

	case WM_DROPFILES:
	{
		HDROP drop = reinterpret_cast< HDROP >(wParam);

		std::vector< unify::Path > files;
		unify::V2< float > point;

		size_t numberOfFiles = DragQueryFile( drop, 0xFFFFFFFF, 0, 0 );
		for ( size_t file = 0; file < numberOfFiles; ++file )
		{
			char filePath[260];
			DragQueryFileA( drop, file, filePath, 260 );
			files.push_back( filePath );
		}

		POINT pt;
		DragQueryPoint( drop, &pt );
		point.x = static_cast< float >(pt.x);
		point.y = static_cast< float >(pt.y);

		game.OnDragDrop( files, point );
	}
	break;
	*/
	}

	return DefWindowProc( handle, message, wParam, lParam );
}