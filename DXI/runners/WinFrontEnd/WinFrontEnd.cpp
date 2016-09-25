// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/win/WindowsOS.h>
#include <chrono>			   
#include <dxi/win/DXILib.h>


using namespace dxi;
using namespace core;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow )
{
	Game game( "setup_luago.xml" );

	MSG msg;
#define NO_TRY

#ifndef NO_TRY
	try
#endif
	{
		// Windows OS specific...
		std::shared_ptr< dxi::win::WindowsOS > windowsOS{ new dxi::win::WindowsOS( hInstance, hPrevInstance, lpszCmdLine, nCmdShow ) };

		dxi::core::Game & game = *dxi::core::Game::GetInstance();

		if( !game.Initialize( windowsOS ) )
		{
			return 0;
		}

		game.Startup();

		const bool forever = true;
		while( forever )
		{
			while( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) == 1 )
			{
				if( !IsDialogMessage( windowsOS->GetHWnd(), &msg ) )
				{
					if( msg.message == WM_QUIT )
					{
						break;
					}
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
			}

			using namespace std::chrono;
			static high_resolution_clock::time_point lastTime = high_resolution_clock::now();
			high_resolution_clock::time_point currentTime = high_resolution_clock::now();
			duration< float > elapsed_d = duration_cast<duration< float >>( currentTime - lastTime );
			auto micro = duration_cast< microseconds >(currentTime - lastTime).count();
			unify::Seconds elapsed = micro * 0.000001;
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

