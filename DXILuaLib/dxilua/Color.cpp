// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/Color.h>

int PushColor( lua_State * state, unify::ColorUnit color )
{
	lua_newtable( state ); // Create table.

	lua_pushstring( state, "r" );
	lua_pushnumber( state, color.r );
	lua_settable( state, -3 );

	lua_pushstring( state, "g" );
	lua_pushnumber( state, color.g );
	lua_settable( state, -3 );

	lua_pushstring( state, "b" );
	lua_pushnumber( state, color.b );
	lua_settable( state, -3 );

	lua_pushstring( state, "a" );
	lua_pushnumber( state, color.a );
	lua_settable( state, -3 );
	return 1;
}

unify::ColorUnit CheckColor( lua_State * state, int index )
{
	luaL_checktype( state, index, LUA_TTABLE );
	
	lua_getfield( state, index, "r" );
	float r = (float)luaL_checknumber( state, -1 );
	
	lua_getfield( state, index, "g" );
	float g = (float)luaL_checknumber( state, -1 );

	lua_getfield( state, index, "b" );
	float b = (float)luaL_checknumber( state, -1 );

	lua_getfield( state, index, "a" );
	float a = (float)luaL_checknumber( state, -1 );

	lua_pop( state, 4 );

	return unify::ColorUnit::ColorUnitRGBA( r, g, b, a );
}
