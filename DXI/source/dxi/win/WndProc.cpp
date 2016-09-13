// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <shellapi.h>

extern "C"
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	static bool trackingMouse = false;
	static dxi::core::Game & game = *dxi::core::Game::GetInstance();

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
		input.SetLeftMouse( true );
	}
	break;

	case WM_LBUTTONUP:
	{
		dxi::core::IInput & input = game.GetInput();
		input.SetLeftMouse( false );
	}
	break;

	case WM_RBUTTONDOWN:
	{
		dxi::core::IInput & input = game.GetInput();
		input.SetRightMouse( true );
	}
	break;

	case WM_RBUTTONUP:
	{
		dxi::core::IInput & input = game.GetInput();
		input.SetRightMouse( false );
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
			trackMouseEvent.hwndTrack = hWnd;
			trackMouseEvent.dwHoverTime = HOVER_DEFAULT;
			TrackMouseEvent( &trackMouseEvent );
		}

		dxi::core::IInput & input = game.GetInput();

		RECT clientRect;
		GetClientRect( hWnd, &clientRect );

		float width = static_cast< float >(game.GetOS().GetResolution().width);
		float height = static_cast< float >(game.GetOS().GetResolution().height);
		float clientWidth = static_cast< float >(clientRect.right);
		float clientHeight = static_cast< float >(clientRect.bottom);

		unify::V2< int > mousePosition( static_cast< int >(LOWORD( lParam )), static_cast< int >(HIWORD( lParam )) );
		mousePosition.x *= static_cast< int >(width / clientWidth);
		mousePosition.y *= static_cast< int >(height / clientHeight);
		input.SetMousePosition( mousePosition );
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

	return DefWindowProc( hWnd, message, wParam, lParam );
}
