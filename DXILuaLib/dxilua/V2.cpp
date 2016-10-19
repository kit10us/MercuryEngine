// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/V2.h>

// Helper functions...

unify::V2< float > CheckV2( lua_State * state, int index )
{
	luaL_checktype( state, index, LUA_TTABLE );
	
	lua_getfield( state, index, "x" );
	lua_getfield( state, index, "y" );

	float x = (float)luaL_checknumber( state, -2 );
	float y = (float)luaL_checknumber( state, -1 );

	lua_pop( state, 2 );

	return unify::V2< float >( x, y );
}

int PushV2( lua_State * state, unify::V2< float > V2 )
{
	lua_newtable( state ); // Create table.

	lua_pushstring( state, "x" );
	lua_pushnumber( state, V2.x );
	lua_settable( state, -3 );

	lua_pushstring( state, "y" );
	lua_pushnumber( state, V2.y );
	lua_settable( state, -3 );

	return 1;
}			 
