// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ExportGame.h>
#include <dxilua/DXILua.h>

using namespace dxilua;
using namespace dxi;

int Game_GetWidth( lua_State * state )
{
	int args = lua_gettop( state );
	int renderer = 0;
	if ( args == 1 )
	{
		renderer = lua_tonumber( state, 1 );
	}

	auto game = ScriptEngine::GetGame();

	lua_pushnumber( state, game->GetOS().GetRenderer( renderer )->GetViewport().GetWidth() );

	return 1;
}

int Game_GetHeight( lua_State * state )
{
	int args = lua_gettop( state );
	int renderer = 0;
	if( args == 1 )
	{
		renderer = lua_tonumber( state, 1 );
	}

	auto game = ScriptEngine::GetGame();

	lua_pushnumber( state, game->GetOS().GetRenderer( renderer )->GetViewport().GetHeight() );

	return 1;
}

int Game_GetRendererCount( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	auto game = ScriptEngine::GetGame();

	float result = game->GetOS().RendererCount();

	lua_pushnumber( state, result );

	return 1;
}

int Game_Quit( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	auto game = ScriptEngine::GetGame();

	game->RequestQuit();

	return 0;
}

static const luaL_Reg gameFuncs[] =
{
	{ "GetWidth", Game_GetWidth },
	{ "GetHeight", Game_GetHeight },
	{ "GetRendererCount", Game_GetRendererCount },
	{ "Quit", Game_Quit },
	{ nullptr, nullptr }
};

int ExportGame( lua_State * state )
{
	luaL_newlib( state, gameFuncs );
	lua_setglobal( state, "game" );
	return 1;
}
