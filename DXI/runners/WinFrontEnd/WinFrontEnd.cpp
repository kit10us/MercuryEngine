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
std::shared_ptr< dxi::win::WindowsOS > g_windowsOS;

extern "C"
LRESULT CALLBACK WndProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	static bool trackingMouse = false;
	static dxi::core::Game & game = *g_game;

	return g_windowsOS->WndProc( handle, message, wParam, lParam );
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
		g_windowsOS.reset( new dxi::win::WindowsOS( &game, hInstance, hPrevInstance, lpszCmdLine, nCmdShow, WndProc ) );

		if( !game.Initialize( g_windowsOS ) )
		{
			return 0;
		}

		const bool forever = true;
		while( forever )
		{
			while( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) == 1 )
			{
				// TODO:
				if( !IsDialogMessage( g_windowsOS->GetHandle(), &msg ) )
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

