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

