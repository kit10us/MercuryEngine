#pragma once

#include <dxilua/Util.h>

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
