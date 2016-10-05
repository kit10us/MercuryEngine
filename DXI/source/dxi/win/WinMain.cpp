// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/win/WindowsOS.h>
#include <dxi/core/Game.h>
#include <chrono>

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow )
{
	MSG msg;
#define NO_TRY

#ifndef NO_TRY
	try
#endif
	{
		// Windows OS specific...
		std::shared_ptr< dxi::win::WindowsOS > windowsOS { new dxi::win::WindowsOS( hInstance, hPrevInstance, lpszCmdLine, nCmdShow ) };

		dxi::core::Game & game = *dxi::core::Game::GetInstance();

		if ( ! game.Initialize( windowsOS ) )
		{
			return 0;
		}

		game.Startup();

		const bool forever = true;
		while( forever )
		{ 
			while( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) == 1 )
			{
				if ( ! IsDialogMessage( windowsOS->GetHWnd(), &msg ) )
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

		game.Shutdown();
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