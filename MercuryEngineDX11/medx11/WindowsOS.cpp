// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <medx11/Debug.h>
#include <medx11/WindowsOS.h>
#include <medx11/Renderer.h>
#include <unify/Exception.h>
#include <unify/Path.h>
#include <me/exception/FailedToCreate.h>
#include <shellapi.h>

#include <medx11/DirectX.h>

using namespace me;
using namespace render;
using namespace medx11;

#ifdef GetCommandLine
#undef GetCommandLine
#endif

WindowsOS::WindowsOS( game::IGame * gameInstance, const qxml::Element * element )
	: m_game( gameInstance )
	, m_debug{ new medx11::Debug{} }
	, m_hasFocus{}
	, m_keyboard{}
	, m_mouse{}
	, m_osParameters{ gameInstance->GetOSParameters() }
{	
			
	{
		using namespace std;
		char buffer[MAX_PATH];
		GetModuleFileNameA( NULL, buffer, MAX_PATH );
		m_programPath = unify::Path( buffer );
		m_programPath.Normalize();
		string::size_type pos = string( buffer ).find_last_of( "\\/" );
		if( pos != string::npos ) 
		{
			m_name = buffer;
		}
	}

	{
		char buffer[ MAX_PATH ];
		GetCurrentDirectoryA( MAX_PATH, buffer );
		m_runPath = unify::Path( std::string( buffer ) + "/" );
	}

	// TODO: We are also doing this in Game, shouldn't this be in once place?
	// Parse the commandline...
	std::string commandLineString( m_osParameters.lpszCmdLine );
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

	// Load display setup...
	for( auto && node : element->Children( "display" ) )
	{
		bool fullscreen = node.GetAttributeElse< bool >( "fullscreen", false );
		int width = node.GetAttribute< int >( "width" );
		int height = node.GetAttribute< int >( "height" );
		int x = node.GetAttributeElse< int >( "x", 0 );
		int y = node.GetAttributeElse< int >( "y", 0 );
		float nearZ = node.GetAttributeElse< float >( "nearz", 0.0f );
		float farZ = node.GetAttributeElse< float >( "farz", 1000.0f );

		render::Display display {};
		if ( fullscreen )
		{
			display = render::Display::CreateFullscreenDirectXDisplay( unify::Size< float >( (float)width, (float)height ) );
		}
		else
		{
			display = render::Display::CreateWindowedDirectXDisplay( unify::Size< float >( (float)width, (float)height ), unify::V2< float >( (float)x, (float)y ) );
		}

		display.SetNearZ( nearZ );
		display.SetFarZ( farZ );

		AddDisplay( display );
	}
}

WindowsOS::~WindowsOS()
{
	m_renderers.clear();
}

me::game::IGame * WindowsOS::GetGame()
{
	return m_game;
}

me::IDebug * WindowsOS::Debug()
{
	return m_debug;
}

const me::IDebug * WindowsOS::Debug() const
{
	return m_debug;
}

void * WindowsOS::Feed( std::string target, void * data )
{
	os::win::OSFood * food = (os::win::OSFood*)data;

	return (void*)WndProc( (HWND)food->handle, (UINT)food->message, (WPARAM)food->wParam, (LPARAM)food->lParam );
}

std::string WindowsOS::GetName() const
{
	return m_name;
}

const std::vector< std::string > & WindowsOS::GetCommandLine() const
{
	return m_commandLine;
}

void WindowsOS::AddDisplay( render::Display display )
{
	m_pendingDisplays.push_back( display );
}

void WindowsOS::CreatePendingDisplays( std::string title )
{
	if( m_pendingDisplays.empty() )
	{
		return;
	}

	for( auto && display : m_pendingDisplays )
	{
		CreateDisplay( display, title );
	}

	m_pendingDisplays.clear();
}

void WindowsOS::CreateDisplay( render::Display display, std::string title )
{
	bool isPrimary = m_renderers.empty() ? true : false; // Note that this is VERY explicit - we are actual spelling out our intention (even though it looks redundant).

	std::shared_ptr< Renderer > renderer;

	// If we have no handle, create a window...
	if ( display.GetHandle() == 0 )
	{
		// Regardless of windowed or not, we need a window...
		WNDCLASS wc;
		memset( &wc, 0, sizeof( wc ) );
		wc.style = 0;
		wc.lpfnWndProc = (WNDPROC)m_osParameters.wndProc;
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
		HWND parentHandle = (HWND)display.GetParentHandle();
		HINSTANCE hInstance = GetHInstance();
		HWND handle = CreateWindowA( "MainWndClass", title.c_str(), WS_OVERLAPPEDWINDOW, x, y, width, height,
			parentHandle, (HMENU)NULL, hInstance, (LPVOID)NULL );

		if( !handle )
		{
			throw exception::FailedToCreate( "Failed to create window!" );
		}

		// Resize window to ensure exact pixel match...
		RECT windowRect;
		GetWindowRect( handle, &windowRect );

		RECT clientRect;
		GetClientRect( handle, &clientRect );

		long newWindowWidth = (windowRect.right - windowRect.left) + width - clientRect.right;
		long newWindowHeight = (windowRect.bottom - windowRect.top) + height - clientRect.bottom;
		MoveWindow( handle, windowRect.left, windowRect.top, newWindowWidth, newWindowHeight, false );

		ShowWindow( handle, m_osParameters.nCmdShow );
		UpdateWindow( handle );
		display.SetHandle( handle );
	}

	renderer.reset( new Renderer( this, display, m_renderers.size() ) );
	m_renderers.push_back( renderer );
}

int WindowsOS::RendererCount() const
{
	return m_renderers.size();
}

IRenderer * WindowsOS::GetRenderer( int index) const
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
	return (HINSTANCE)m_osParameters.hInstance;
}

HWND WindowsOS::GetHandle() const
{
	if ( m_osParameters.hWnd )
	{
		return (HWND)m_osParameters.hWnd;
	}
	else
	{
		return (HWND)m_renderers[0]->GetDisplay().GetHandle();
	}
}

void WindowsOS::BuildRenderers( std::string title )
{		 
	CreatePendingDisplays( title );

	// TODO: DragAcceptFiles( this->GetHWnd(), true );
}

void WindowsOS::Startup()
{
	auto keyboardItr = m_game->GetInputManager()->FindSource( "Keyboard" );
	if ( keyboardItr )
	{
		m_keyboard = keyboardItr.get();
	}

	auto mouseItr = m_game->GetInputManager()->FindSource( "Mouse" );
	if ( mouseItr )
	{
		m_mouse = mouseItr.get();
	}
}

void WindowsOS::Shutdown()
{
}

LRESULT WindowsOS::WndProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	game::IGame & gameInstance = *m_game;
	static bool trackingMouse = false;

	switch ( message )
	{
	case WM_CLOSE: // Fall through to WM_DESTROY...
	case WM_DESTROY:
		gameInstance.Quit();
		return 0;

	case WM_MOUSELEAVE:
	{
		if ( m_mouse == nullptr ) break;

		for( int renderer = 0; renderer < RendererCount(); ++renderer )
		{
			if ( GetRenderer( renderer )->GetHandle() == handle )
			{
				trackingMouse = false;
				//m_mouse->SetState( renderer, "MouseAvailable", "Available", false );
				break;
			}
		}
		break;
	}

	case WM_LBUTTONDOWN:
	{
		if ( m_mouse == nullptr ) break;

		for ( int renderer = 0; renderer < RendererCount(); ++renderer )
		{
			if ( GetRenderer( renderer )->GetHandle() == handle )
			{
				trackingMouse = false;
				input::ButtonData * data = new input::ButtonData();
				data->down = true;
				data->pressed = false;
				size_t inputIndex = m_mouse->GetInputIndex( renderer, "LeftButton" );
				m_mouse->SetInputData( renderer, inputIndex, input::IData::ptr( data ) );
				break;
			}
		}
		break;
	}

	case WM_LBUTTONUP:
	{
		if ( m_mouse == nullptr ) break;

		for ( int renderer = 0; renderer < RendererCount(); ++renderer )
		{
			if ( GetRenderer( renderer )->GetHandle() == handle )
			{
				trackingMouse = false;
				input::ButtonData * data = new input::ButtonData();
				data->down = false;
				data->pressed = false;
				size_t inputIndex = m_mouse->GetInputIndex( renderer, "LeftButton" );
				m_mouse->SetInputData( renderer, inputIndex, input::IData::ptr( data ) );
				break;
			}
		}
		break;
	}

	case WM_RBUTTONDOWN:
	{
		if ( m_mouse == nullptr ) break;

		for ( int renderer = 0; renderer < RendererCount(); ++renderer )
		{
			if ( GetRenderer( renderer )->GetHandle() == handle )
			{
				trackingMouse = false;
				input::ButtonData * data = new input::ButtonData();
				data->down = true;
				data->pressed = false;
				size_t inputIndex = m_mouse->GetInputIndex( renderer, "RightButton" );
				m_mouse->SetInputData( renderer, inputIndex, input::IData::ptr( data ) );
				break;
			}
		}
		break;
	}

	case WM_RBUTTONUP:
	{
		if ( m_mouse == nullptr ) break;

		for ( int renderer = 0; renderer < RendererCount(); ++renderer )
		{
			if ( GetRenderer( renderer )->GetHandle() == handle )
			{
				trackingMouse = false;
				input::ButtonData * data = new input::ButtonData();
				data->down = false;
				data->pressed = false;
				size_t inputIndex = m_mouse->GetInputIndex( renderer, "RightButton" );
				m_mouse->SetInputData( renderer, inputIndex, input::IData::ptr( data ) );
				break;
			}
		}
		break;
	}

	case WM_MBUTTONDOWN:
	{
		if ( m_mouse == nullptr ) break;

		for ( int renderer = 0; renderer < RendererCount(); ++renderer )
		{
			if ( GetRenderer( renderer )->GetHandle() == handle )
			{
				trackingMouse = false;
				input::ButtonData * data = new input::ButtonData();
				data->down = true;
				data->pressed = false;
				size_t inputIndex = m_mouse->GetInputIndex( renderer, "MiddleButton" );
				m_mouse->SetInputData( renderer, inputIndex, input::IData::ptr( data ) );
				break;
			}
		}
		break;
	}

	case WM_MBUTTONUP:
	{
		if ( m_mouse == nullptr ) break;

		for ( int renderer = 0; renderer < RendererCount(); ++renderer )
		{
			if ( GetRenderer( renderer )->GetHandle() == handle )
			{
				trackingMouse = false;
				input::ButtonData * data = new input::ButtonData();
				data->down = false;
				data->pressed = false;
				size_t inputIndex = m_mouse->GetInputIndex( renderer, "MiddleButton" );
				m_mouse->SetInputData( renderer, inputIndex, input::IData::ptr( data ) );
				break;
			}
		}
		break;
	}

	case WM_MOUSEWHEEL:
	{
		if ( m_mouse == nullptr ) break;

		short zDelta = GET_WHEEL_DELTA_WPARAM( wParam );
		for ( int renderer = 0; renderer < RendererCount(); ++renderer )
		{
			if ( GetRenderer( renderer )->GetHandle() == handle )
			{
				trackingMouse = false;
				input::TrackerData * data = new input::TrackerData();
				size_t inputIndex = m_mouse->GetInputIndex( renderer, "Tracker" );
				data->set.z = true;
				data->change.z = zDelta / (float)WHEEL_DELTA;
				m_mouse->SetInputData( renderer, inputIndex, input::IData::ptr( data ) );
				break;
			}
		}
		break;
	}

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
								
				float width = static_cast<float>(GetRenderer( renderer )->GetViewport().GetSize().width);
				float height = static_cast< float >(GetRenderer( renderer )->GetViewport().GetSize().height);
				float clientWidth = static_cast< float >(clientRect.right);
				float clientHeight = static_cast< float >(clientRect.bottom);

				unify::V2< int > mousePosition( static_cast< int >(LOWORD( lParam )), static_cast< int >(HIWORD( lParam )) );
				mousePosition.x *= static_cast< int >(width / clientWidth);
				mousePosition.y *= static_cast< int >(height / clientHeight);

				input::TrackerData * data = new input::TrackerData();
				size_t inputIndex = m_mouse->GetInputIndex( renderer, "Tracker" );
				data->set.x = true;
				data->position.x = (float)mousePosition.x;
				data->set.y = true;
				data->position.y = (float)mousePosition.y;
				m_mouse->SetInputData( renderer, inputIndex, input::IData::ptr( data ) );				
				break;
			}
		}
		break;
	}

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

unify::AssetPaths & WindowsOS::GetAssetPaths()
{
	return m_assetPaths;
}

unify::Path WindowsOS::GetProgramPath() const
{
	return m_programPath;
}

unify::Path WindowsOS::GetRunPath() const
{
	return m_runPath;
}
