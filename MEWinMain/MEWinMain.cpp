// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <WndProc.h>
#include <me/game/Game.h>

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#ifdef GetCommandLine
#undef GetCommandLine
#endif // GetCommandLine
#ifdef GetObject
#undef GetObject
#endif
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

#include <shellapi.h>

extern "C" LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );


extern "C" me::game::Game * GetGameInstance();

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow )
{
	MSG msg;

	static me::game::Game * gameInstance;

	try
	{
		gameInstance = GetGameInstance();
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

		bool retry = true;
		while( retry )
		{
			retry = false;
			try
			{
				gameInstance->Initialize( osParameters );
			}
			catch( std::exception exception )
			{
				gameInstance->Debug()->LogLine( "Failure", "Mercury Failure:" );
				gameInstance->Debug()->LogLine( "Failure", exception.what() );
				int result = MessageBoxA( 0, exception.what(), "Mercury Failure", MB_ICONEXCLAMATION | MB_ABORTRETRYIGNORE );
				switch( result )
				{
				case IDABORT:
					return 0;

				case IDRETRY:
					retry = true;
					break;

				case IDIGNORE:
					// Do nothing.
					break;
				}
			}
		}

		bool running = true;
		while( running )
		{ 
			try
			{
				while( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) == 1 )
				{

					if( msg.message == WM_QUIT )
					{
						break;
					}
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}

				gameInstance->Tick();
				if( gameInstance->IsQuitting() )
				{
					break;
				}

				gameInstance->Draw();
			}
			catch( std::exception exception )
			{
				gameInstance->Debug()->LogLine( "Failure", "Mercury Failure: " );
				gameInstance->Debug()->LogLine( "Failure", exception.what() );
				int result = MessageBoxA( 0, exception.what(), "Mercury Failure", MB_ICONEXCLAMATION | MB_ABORTRETRYIGNORE );
				switch( result )
				{
				case IDABORT:
					return -1;
					break;

				case IDRETRY:
					// TODO:
					//break;

				case IDIGNORE:
					// Do nothing.
					break;
				}
			}
		}
	}
	catch( std::exception exception )
	{
		gameInstance->Debug()->LogLine( "Failure", "Mercury Failure:" );
		gameInstance->Debug()->LogLine( "Failure", exception.what() );
		MessageBoxA( 0, exception.what(), "Mercury Failure", MB_ICONEXCLAMATION );
		return -1;
	}
 
    return msg.wParam; 
} 