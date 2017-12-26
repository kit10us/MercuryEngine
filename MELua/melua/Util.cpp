#pragma once

#include <melua/Util.h>

namespace melua
{
	std::string GetTypename( lua_State *L )
	{
		return GetTypename( L, lua_gettop( L ) );
	}

	std::string GetTypename( lua_State *L, int index )
	{
		int top = lua_gettop( L );
		int type = lua_type( L, index );
		switch( type )
		{
		default:
		case LUA_TNONE:
			return "none";
		case LUA_TNIL:
			return "nil";
		case LUA_TBOOLEAN:
			return "boolean";
		case LUA_TLIGHTUSERDATA:
			return "lightuserdata";
		case LUA_TNUMBER:
			return "number";
		case LUA_TSTRING:
			return "string";
		case LUA_TTABLE:
			return "table";
		case LUA_TFUNCTION:
			return "function";
		case LUA_TUSERDATA:
		{
			lua_getmetatable( L, index );
			int subType = lua_type( L, -1 );

			if( subType != LUA_TTABLE )
			{
				lua_pop( L, 1 );
				assert( top == lua_gettop( L ) );
				return std::string();
			}
			else
			{
				lua_getfield( L, -1, "_type" );
				type = lua_type( L, -1 );
				if( type != LUA_TSTRING )
				{
					lua_pop( L, 2 );
					assert( top == lua_gettop( L ) );
					return std::string();
				}
				else
				{
					std::string typeName = lua_tostring( L, -1 );
					lua_pop( L, 2 );
					assert( top == lua_gettop( L ) );
					return typeName;
				}
			}
		}
		case LUA_TTHREAD:
			return "thread";
		}
	}

	std::vector< std::string > GetTypenames( lua_State *L )
	{
		std::vector< std::string > types;
		for( int arg = 1, args = lua_gettop( L ); arg <= args; arg++ )
		{
			types.push_back( GetTypename( L, arg ) );
		}
		return types;
	}

	int PushNil( lua_State *L )
	{
		lua_pushnil( L );
		return 1;
	}

	MELUADLL_API void Error( lua_State *L, std::string error )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();
		auto gameInstance = se->GetGame();
		gameInstance->ReportError( me::ErrorLevel::Failure, "Lua", error );
		luaL_error( L, error.c_str() );
	}

	template<>
	bool Check( lua_State *L, int index )
	{
		return lua_toboolean( L, index ) ? true : false;
	}

	template<>
	int Check( lua_State *L, int index )
	{
		return (int)lua_tonumber( L, index );
	}

	template<>
	float Check( lua_State *L, int index )
	{
		return (float)lua_tonumber( L, index );
	}

	template<>
	std::string Check( lua_State *L, int index )
	{
		return lua_tostring( L, index );
	}

	template<>
	int Push( lua_State * L, bool value )
	{
		lua_pushboolean( L, value ? 1 : 0 );
		return 1;
	}

	template<>
	int Push( lua_State * L, std::string value )
	{
		lua_pushstring( L, value.c_str() );
		return 1;
	}

	template<>
	int Push( lua_State * L, float value )
	{
		lua_pushnumber( L, value ? 1 : 0 );
		return 1;
	}

	template<>
	int Push( lua_State * L, int value )
	{
		lua_pushnumber( L, value );
		return 1;
	}

	template<>
	int Push( lua_State * L, unsigned int value )
	{
		lua_pushboolean( L, value ? 1 : 0 );
		return 1;
	}
}


