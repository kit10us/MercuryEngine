// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ExportGame.h>
#include <dxilua/DXILua.h>

using namespace dxilua;
using namespace dxi;

extern "C"
int Game_GetWidth( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	auto game = ScriptEngine::GetGame();

	lua_pushnumber( state, game->GetOS().GetRenderer( 0 )->GetViewport().GetWidth() );

	return 1;
}

extern "C"
int Game_GetHeight( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	auto game = ScriptEngine::GetGame();

	lua_pushnumber( state, game->GetOS().GetRenderer( 0 )->GetViewport().GetHeight() );

	return 1;
}

extern "C"
int Game_GetAspectRatioHW( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	auto game = ScriptEngine::GetGame();

	float hw = game->GetOS().GetRenderer( 0 )->GetViewport().GetHeight() / game->GetOS().GetRenderer( 0 )->GetViewport().GetWidth();

	lua_pushnumber( state, hw );

	return 1;
}

static const luaL_Reg gameFuncs[] =
{
	{ "getwidth", Game_GetWidth },
	{ "getheight", Game_GetHeight },
	{ "getaspectratiohw", Game_GetAspectRatioHW },
	{ nullptr, nullptr }
};

int ExportGame( lua_State * state )
{
	luaL_newlib( state, gameFuncs );
	lua_setglobal( state, "game" );
	return 1;
}
