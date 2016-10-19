// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/Size2.h>

// Helper functions...

unify::Size< float > CheckSize2( lua_State * state, int index )
{
	luaL_checktype( state, index, LUA_TTABLE );
	
	lua_getfield( state, index, "width" );
	lua_getfield( state, index, "height" );

	float width = (float)luaL_checknumber( state, -2 );
	float height = (float)luaL_checknumber( state, -1 );

	lua_pop( state, 2 );

	return unify::Size< float >( width, height );
}

int PushSize2( lua_State * state, unify::Size< float > size3 )
{
	lua_newtable( state ); // Create table.

	lua_pushstring( state, "width" );
	lua_pushnumber( state, size3.width );
	lua_settable( state, -3 );

	lua_pushstring( state, "height" );
	lua_pushnumber( state, size3.height );
	lua_settable( state, -3 );
	return 1;
}			 
