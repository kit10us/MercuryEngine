// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/win/WindowsOS.h>
#include <chrono>			   
#include <dxi/win/DXILib.h>
#include <shellapi.h>

using namespace dxi;
using namespace core;

Game * g_game = {};

extern "C"
LRESULT CALLBACK WndProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	static bool trackingMouse = false;
	static dxi::core::Game & game = *g_game;

	switch( message )
	{
	case WM_CLOSE: // Fall through to WM_DESTROY...
	case WM_DESTROY:
		game.RequestQuit();
		return 0;

	case WM_MOUSELEAVE:
	{
		trackingMouse = false;
		dxi::core::IInput & input = game.GetInput();
		trackingMouse = false;
		input.SetMouseUnavailable();
	}
	break;

	case WM_LBUTTONDOWN:
	{
		dxi::core::IInput & input = game.GetInput();
		input.SetLeftMouse( handle, true );
	}
	break;

	case WM_LBUTTONUP:
	{
		dxi::core::IInput & input = game.GetInput();
		input.SetLeftMouse( handle, false );
	}
	break;

	case WM_RBUTTONDOWN:
	{
		dxi::core::IInput & input = game.GetInput();
		input.SetRightMouse( handle, true );
	}
	break;

	case WM_RBUTTONUP:
	{
		dxi::core::IInput & input = game.GetInput();
		input.SetRightMouse( handle, false );
	}
	break;

	case WM_MOUSEMOVE:
	{
		// Enable tracking when the mouse leaves the client area...
		if( !trackingMouse )
		{
			TRACKMOUSEEVENT trackMouseEvent = TRACKMOUSEEVENT();
			trackMouseEvent.cbSize = sizeof TRACKMOUSEEVENT;
			trackMouseEvent.dwFlags = TME_LEAVE;
			trackMouseEvent.hwndTrack = handle;
			trackMouseEvent.dwHoverTime = HOVER_DEFAULT;
			TrackMouseEvent( &trackMouseEvent );
		}

		dxi::core::IInput & input = game.GetInput();

		RECT clientRect;
		GetClientRect( handle, &clientRect );

		dxi::core::IRenderer * renderer = {};
		for ( int i = 0; i < game.GetOS().RendererCount(); ++i )
		{
			if ( game.GetOS().GetRenderer( i )->GetHandle() == handle )
			{
				renderer = game.GetOS().GetRenderer( i );
				break;
			}
		}		

		float width = static_cast<float>(renderer->GetViewport().GetWidth());
		float height = static_cast< float >(renderer->GetViewport().GetHeight());
		float clientWidth = static_cast< float >(clientRect.right);
		float clientHeight = static_cast< float >(clientRect.bottom);

		unify::V2< int > mousePosition( static_cast< int >(LOWORD( lParam )), static_cast< int >(HIWORD( lParam )) );
		mousePosition.x *= static_cast< int >(width / clientWidth);
		mousePosition.y *= static_cast< int >(height / clientHeight);
		input.SetMousePosition( handle, mousePosition );
	}
	break;

	case WM_ACTIVATE:
	{
		WORD lowOrder = wParam & 0x0000FFFF;
		bool minimized = (wParam & 0xFFFF0000) != 0;
		switch( lowOrder )
		{
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
			game.GetOS().SetHasFocus( true );
			break;
		case WA_INACTIVE:
			game.GetOS().SetHasFocus( false );
			break;

		default:
			assert( 0 && "Invalid activity state!" );
		}
	}
	break;

	case WM_DROPFILES:
	{
		HDROP drop = reinterpret_cast< HDROP >(wParam);

		std::vector< unify::Path > files;
		unify::V2< float > point;

		size_t numberOfFiles = DragQueryFile( drop, 0xFFFFFFFF, 0, 0 );
		for( size_t file = 0; file < numberOfFiles; ++file )
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
	}

	return DefWindowProc( handle, message, wParam, lParam );
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow )
{
	Game game( "setup_luago.xml" );
	g_game = &game;

	MSG msg;
#define NO_TRY

#ifndef NO_TRY
	try
#endif
	{
		// Windows OS specific...
		std::shared_ptr< dxi::win::WindowsOS > windowsOS{ new dxi::win::WindowsOS( hInstance, hPrevInstance, lpszCmdLine, nCmdShow, WndProc ) };

		if( !game.Initialize( windowsOS ) )
		{
			return 0;
		}

		const bool forever = true;
		while( forever )
		{
			while( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) == 1 )
			{
				// TODO:
				if( !IsDialogMessage( windowsOS->GetHandle(), &msg ) )
				{
					if( msg.message == WM_QUIT )
					{
						break;
					}
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
			}

			game.Tick();
			if( game.IsQuitting() )
			{
				break;
			}

			game.Draw();
		}	 
	}
#ifndef NO_TRY
	catch( std::exception & exception )
	{
		OutputDebugStringA( "[" );
		OutputDebugStringA( "exception: " );
		OutputDebugStringA( exception.what() );
		OutputDebugStringA( "]\n" );
		MessageBoxA( 0, exception.what(), "Exception caught", MB_ICONEXCLAMATION );
		return -1;
	}
#endif

	return msg.wParam;
}

