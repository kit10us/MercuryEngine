// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/V3.h>

// Helper functions...

unify::V3< float > CheckV3( lua_State * state, int index )
{
	luaL_checktype( state, index, LUA_TTABLE );
	
	lua_getfield( state, index, "x" );
	lua_getfield( state, index, "y" );
	lua_getfield( state, index, "z" );

	float x = (float)luaL_checknumber( state, -3 );
	float y = (float)luaL_checknumber( state, -2 );
	float z = (float)luaL_checknumber( state, -1 );

	lua_pop( state, 3 );

	return unify::V3< float >( x, y, z );
}

int PushV3( lua_State * state, unify::V3< float > v3 )
{
	lua_newtable( state ); // Create table.

	lua_pushstring( state, "x" );
	lua_pushnumber( state, v3.x );
	lua_settable( state, -3 );

	lua_pushstring( state, "y" );
	lua_pushnumber( state, v3.y );
	lua_settable( state, -3 );

	lua_pushstring( state, "z" );
	lua_pushnumber( state, v3.z );
	lua_settable( state, -3 );
	return 1;
}			 

