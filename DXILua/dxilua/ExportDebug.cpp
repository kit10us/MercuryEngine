// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ExportDebug.h>
#include <dxilua/ScriptEngine.h>

using namespace dxilua;
using namespace me;

int Debug_LogLine( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	int type = lua_type( state, 1 );
	std::string typeName( lua_typename( state, type ) );

	auto game = ScriptEngine::GetGame();
	if ( type == LUA_TNIL )
	{
		game->LogLine( "<NIL>\n" );
	}
	else if ( type == LUA_TUSERDATA )
	{
		game->LogLine( "<USERDATA>\n" );
	}
	else
	{
		std::string log = lua_tostring( state, 1 );			  
		game->LogLine( log );
	}

	return 0;
}

int ExportDebug( lua_State * state )
{
	static const luaL_Reg debugFuncs[] =
	{
		{ "LogLine", Debug_LogLine },
		{ nullptr, nullptr }
	};

	luaL_newlib( state, debugFuncs );
	lua_setglobal( state, "Debug" );

	lua_register( state, "print", Debug_LogLine );

	return 1;
}

