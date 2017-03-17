#pragma once

#include <melua/Util.h>

void ShowStack( lua_State *L )
{
	int i;
	int top = lua_gettop( L );
	for ( i = 1; i <= top; i++ ) { // repeat for each level
		int t = lua_type( L, i );
		switch ( t ) {

		case LUA_TSTRING: // strings
			OutputDebugStringA( lua_tostring( L, i ) );
			break;

		case LUA_TBOOLEAN: // booleans
			OutputDebugStringA( lua_toboolean( L, i ) ? "true" : "false" );
			break;

		case LUA_TNUMBER: // numbers
			OutputDebugStringA( unify::Cast< std::string>( lua_tonumber( L, i ) ).c_str() );
			break;

		default: // other values
			OutputDebugStringA( lua_typename( L, t ) );
			break;

		}
		OutputDebugStringA( " " ); // put a separator
	}
	OutputDebugStringA( "\n" ); // end the listing
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
		lua_getmetatable( L, index );
		type = lua_type( L, index + 1 );
		if ( type != LUA_TTABLE )
		{
			return std::string();
		}
		lua_getfield( L, index + 1, "_type" );
		type = lua_type( L, index + 2 );
		top = lua_gettop( L );
		if ( type != LUA_TSTRING )
		{
			lua_pop( L, 2 );
			return std::string();
		}
		top = lua_gettop( L );
		{
			std::string typeName = lua_tostring( L, index + 2 );
			top = lua_gettop( L );
			lua_pop( L, 2 );
			return typeName;
		}
	case LUA_TTHREAD:
		return "thread";
	}
}