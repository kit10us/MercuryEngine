// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/Size3.h>

// Helper functions...

unify::Size3< float > CheckSize3( lua_State * state, int index )
{
	luaL_checktype( state, index, LUA_TTABLE );
	
	lua_getfield( state, index, "width" );
	lua_getfield( state, index, "height" );
	lua_getfield( state, index, "depth" );

	float width = (float)luaL_checknumber( state, -3 );
	float height = (float)luaL_checknumber( state, -2 );
	float depth = (float)luaL_checknumber( state, -1 );

	lua_pop( state, 3 );

	return unify::Size3< float >( width, height, depth );
}

int PushSize3( lua_State * state, unify::Size3< float > size3 )
{
	lua_newtable( state ); // Create table.

	lua_pushstring( state, "width" );
	lua_pushnumber( state, size3.width );
	lua_settable( state, -3 );

	lua_pushstring( state, "height" );
	lua_pushnumber( state, size3.height );
	lua_settable( state, -3 );

	lua_pushstring( state, "depth" );
	lua_pushnumber( state, size3.depth );
	lua_settable( state, -3 );
	return 1;
}			 

