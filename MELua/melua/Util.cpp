#pragma once

#include <melua/Util.h>

std::string GetTypename( lua_State *L )
{
	return GetTypename( L, lua_gettop( L ) );
}

std::string GetTypename( lua_State *L, int index )
{
	int top = lua_gettop( L );
	int type = lua_type( L, index );
	switch ( type )
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

		if ( subType != LUA_TTABLE )
		{
			lua_pop( L, 1 );
			assert( top == lua_gettop( L ) );
			return std::string();
		}
		else
		{
			lua_getfield( L, -1, "_type" );
			type = lua_type( L, -1 );
			if ( type != LUA_TSTRING )
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