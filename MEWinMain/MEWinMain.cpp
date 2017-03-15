// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <WndProc.h>
#include <me/Game.h>
#include <shellapi.h>

extern "C" me::Game * GetGameInstance();

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow )
{
	MSG msg;

	static me::Game * game;

	try
	{
		game = GetGameInstance();
	}
	catch( std::exception exception )
	{
		// NOTE: Our goal is to never hit hear in release.
        OutputDebugStringA( "[" );
        OutputDebugStringA( "Mercury Failure: " );
        OutputDebugStringA( exception.what() );
        OutputDebugStringA( "]\n" );
		MessageBoxA( 0, exception.what(), "Mercury Failure", MB_ICONEXCLAMATION );
		return -1;
	}

	try
	{
		me::OSParameters osParameters;
		osParameters.hInstance = hInstance;
		osParameters.hPrevInstance = hPrevInstance;
		osParameters.lpszCmdLine = lpszCmdLine;
		osParameters.nCmdShow = nCmdShow;
		osParameters.wndProc = WndProc; 

		if ( ! game->Initialize( osParameters ) )
		{
			return 0;
		}

		const bool forever = true;
		while( forever )
		{ 
			while( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) == 1 )
			{

				//if ( ! IsDialogMessage( windowsOS->GetHandle(), &msg ) )
				{
					if( msg.message == WM_QUIT )
					{
						break;
					}
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
			}

			game->Tick();
			if( game->IsQuitting() )
			{
				break;
			}
			
			game->Draw();
		} 
	}
	catch( std::exception exception )
	{
		game->LogLine( "Mercury Failure: " );
		game->LogLine( exception.what(), 4 );
		MessageBoxA( 0, exception.what(), "Mercury Failure", MB_ICONEXCLAMATION );
		return -1;
	}
 
    return msg.wParam; 
} 