// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/ExportDebug.h>
#include <melua/ScriptEngine.h>
#include <melua/Util.h>

using namespace melua;
using namespace me;

int Debug_LogLine( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	int type = lua_type( state, 1 );
	std::string typeName( lua_typename( state, type ) );

	ScriptEngine * se = ScriptEngine::GetInstance();
	auto game = se->GetGame();

	switch( type )
	{
	case LUA_TNIL:
		game->LogLine( "<NIL>\n" );
		break;
	case LUA_TTABLE:
		game->LogLine( "<" + GetTypename( state, 1 ) + ">\n" );
		break;
	case LUA_TUSERDATA:
		game->LogLine( "<" + GetTypename( state, 1 ) + ">\n" );
		break;
	default:
		game->LogLine( lua_tostring( state, 1 ) );
		break;
	}

	return 0;
}

int Debug_Type( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	lua_pushstring( state, GetTypename( state, 1 ).c_str() );

	return 1;
}

int Debug_ToString( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ScriptEngine * se = ScriptEngine::GetInstance();
	auto game = se->GetGame();

	int type = lua_type( state, 1 );
	std::string typeName( lua_typename( state, type ) );

	switch( type )
	{
	case LUA_TNIL:
		game->LogLine( "<NIL>\n" );
		break;
	case LUA_TTABLE:
		game->LogLine( "<" + GetTypename( state, 1 ) + ">\n" );
		break;
	case LUA_TUSERDATA:

	{
		int r2 = lua_getfield( state, 1, "ToString" ); // Get userdata's ToString function.
		lua_pushvalue( state, 1 );  // Copy the previous userdata to the top of the stack, to use as a function parameter.

		if ( r2 != 0 )
		{

			try
			{
				if ( lua_pcall( state, 1, 1, 0 ) != 0 )
				{
					assert( 0 );
				}
			}
			catch ( std::exception ex )
			{
				assert( 0 );
			}
		}
		else
		{
			lua_pop( state, 1 );
		}
		break;
	}
	default:
		lua_tostring( state, 1 );
		break;
	}

	return 1;
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
	lua_register( state, "type", Debug_Type );
	lua_register( state, "tostring", Debug_ToString );

	return 1;
}

