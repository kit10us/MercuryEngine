// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ExportScenes.h>
#include <dxilua/DXILua.h>

using namespace dxilua;
using namespace dxi;

extern "C"
int Update_GetDelta( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	auto game = ScriptEngine::GetGame();
															  
	lua_pushnumber( state, game->GetRenderInfo().GetDelta() );

	return 1;
}

static const luaL_Reg updateFuncs[] =
{
	{ "getdelta", Update_GetDelta },
	{ nullptr, nullptr }
};

int ExportUpdate( lua_State * state )
{
	luaL_newlib( state, updateFuncs );
	lua_setglobal( state, "update" );
	return 1;
}

