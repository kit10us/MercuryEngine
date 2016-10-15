// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/win/WindowsOS.h>
#include <dxi/core/Game.h>
#include <shellapi.h>

extern "C" dxi::core::Game * GetGameInstance();

extern "C"
LRESULT CALLBACK WndProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	static bool trackingMouse = false;
	static dxi::core::Game & game = *GetGameInstance();
	static dxi::win::WindowsOS * windowsOS = dynamic_cast< dxi::win::WindowsOS*>( game.GetOS() );

	return windowsOS->WndProc( handle, message, wParam, lParam );
}


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow )
{
	MSG msg;
	try
	{
		static dxi::core::Game & game = *GetGameInstance();

		// Windows OS specific...
		std::shared_ptr< dxi::win::WindowsOS > windowsOS { new dxi::win::WindowsOS( &game, hInstance, hPrevInstance, lpszCmdLine, nCmdShow, WndProc ) };

		if ( ! game.Initialize( windowsOS ) )
		{
			return 0;
		}

		const bool forever = true;
		while( forever )
		{ 
			while( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) == 1 )
			{
				if ( ! IsDialogMessage( windowsOS->GetHandle(), &msg ) )
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
	catch( std::exception & exception )
	{
		// NOTE: Our goal is to never hit hear in release.
        OutputDebugStringA( "[" );
        OutputDebugStringA( "exception: " );
        OutputDebugStringA( exception.what() );
        OutputDebugStringA( "]\n" );
		MessageBoxA( 0, exception.what(), "Exception caught", MB_ICONEXCLAMATION );
		return -1;
	}
 
    return msg.wParam; 
} 