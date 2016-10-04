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

	g_game->Startup();

	return false;
}

bool __cdecl Tick( HWND hwnd )
{
	using namespace std::chrono;

	auto && game = *g_game;

	try
	{
		static high_resolution_clock::time_point lastTime = high_resolution_clock::now();
		high_resolution_clock::time_point currentTime = high_resolution_clock::now();
		duration< float > elapsed_d = duration_cast<duration< float >>(currentTime - lastTime);
		auto micro = duration_cast< microseconds >(currentTime - lastTime).count();
		unify::Seconds elapsed = micro * 0.000001f;
		lastTime = currentTime;

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
	if( !g_game ) return;

	g_game->Shutdown();
	delete g_game;
	g_game = nullptr;
}
