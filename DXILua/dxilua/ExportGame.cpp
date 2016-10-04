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

	lua_pushnumber( state, game->GetOS().GetResolution().width );

	return 1;
}

extern "C"
int Game_GetHeight( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	auto game = ScriptEngine::GetGame();

	lua_pushnumber( state, game->GetOS().GetResolution().height );

	return 1;
}

extern "C"
int Game_GetAspectRatioHW( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	auto game = ScriptEngine::GetGame();

	lua_pushnumber( state, game->GetOS().GetResolution().AspectRatioHW() );

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
