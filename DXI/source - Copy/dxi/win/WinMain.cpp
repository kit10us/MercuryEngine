// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/win/WindowsOS.h>
#include <dxi/core/Game.h>
#include <mmsystem.h>

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow )
{
	MSG msg;
	try
	{
		// Windows OS specific...
		dxi::win::WindowsOS * windowsOS = new dxi::win::WindowsOS( hInstance, hPrevInstance, lpszCmdLine, nCmdShow );

		dxi::core::Game & game = *dxi::core::Game::GetGameInstance();

		if ( ! game.Initialize( windowsOS ) )
		{
			return 0;
		}

		game.Startup();

		while( 1 )
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

			static unsigned int lastTime = timeGetTime();
			unsigned int currentTime = timeGetTime();
			unify::Seconds elapsed = (currentTime - lastTime) * 0.001f;
			lastTime = currentTime;

			game.BeforeUpdate();
			game.Update( elapsed, game.GetInput() );
			game.AfterUpdate();
			if( game.IsQuitting() )
			{
				break;
			}

			game.BeforeRender();
			game.Render();
			game.AfterRender();
		} 

		game.Shutdown();
	}
	catch( std::exception & exception )
	{
        OutputDebugStringA( "[" );
        OutputDebugStringA( "exception: " );
        OutputDebugStringA( exception.what() );
        OutputDebugStringA( "]\n" );
		MessageBoxA( 0, exception.what(), "Exception caught", MB_ICONEXCLAMATION );
		return -1;
	}
 
    return msg.wParam; 
} 