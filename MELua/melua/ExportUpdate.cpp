// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>

using namespace melua;
using namespace me;

int Update_GetDelta( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	ScriptEngine * se = ScriptEngine::GetInstance();
	auto game = se->GetGame();

	lua_pushnumber( state, game->GetRenderInfo().GetDelta() );

	return 1;
}

static const luaL_Reg updateFuncs[] =
{
	{ "GetDelta", Update_GetDelta },
	{ nullptr, nullptr }
};

int ExportUpdate( lua_State * state )
{
	luaL_newlib( state, updateFuncs );
	lua_setglobal( state, "Update" );
	return 1;
}

