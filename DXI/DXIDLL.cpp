// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include "DXIDLL.h"
#include <dxi/win/DXILib.h>
#include <dxi/core/Game.h>
#include <dxi/win/WindowsOS.h>
#include <chrono>

using namespace dxi;

core::Game * g_game {};


bool __cdecl Startup( HWND hWnd, char * setup )
{
	if( g_game ) return false;

	auto os = std::shared_ptr< core::IOS >( new win::WindowsOS( hWnd ) );

	g_game = new core::Game( setup );

	if( !g_game->Initialize( os ) )
	{
		return 0;
	}

	return false;
}

bool __cdecl Tick( HWND hwnd )
{
	using namespace std::chrono;

	auto && game = *g_game;

	try
	{
		game.Tick();
		if( game.IsQuitting() )
		{
			return false;
		}
		game.Draw();

	}
	catch( std::exception ex )
	{
		game.LogLine( std::string( "Error: " ) + ex.what() );
		return false;
	}

	return true;
}

void __cdecl Shutdown( HWND hWnd )
{
	delete g_game;
	g_game = nullptr;
}
