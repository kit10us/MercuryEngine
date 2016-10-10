// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/unify/ExportSize2.h>
#include <dxilua/DXILua.h>

using namespace dxilua;
using namespace dxi;

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

// Member functions...

int Size2_New( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	float width = (float)lua_tonumber( state, 1 );
	float height = (float)lua_tonumber( state, 2 );

	PushSize2( state, unify::Size< float >( width, height ) );

	return 1;
}

int Size2_Zero( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	PushSize2( state, unify::Size< float >( 0, 0 ) );

	return 1;
}

int Size2_ToString( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::Size< float > size( CheckSize2( state, 1 ) );

	lua_pushstring( state, size.ToString().c_str() );
	return 1;
}

int Size2_Add( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::Size< float > l( CheckSize2( state, 1 ) );
	unify::Size< float > r( CheckSize2( state, 2 ) );
	unify::Size< float > result( l + r );

	PushSize2( state, result );
	return 1;
}

int Size2_Sub( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::Size< float > l( CheckSize2( state, 1 ) );
	unify::Size< float > r( CheckSize2( state, 2 ) );
	unify::Size< float > result( l - r );

	PushSize2( state, result );
	return 1;
}

int Size2_Mul( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::Size< float > l( CheckSize2( state, 1 ) );
	unify::Size< float > r( CheckSize2( state, 2 ) );
	unify::Size< float > result( l * r );

	PushSize2( state, result );
	return 1;
}

int Size2_Div( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::Size< float > l( CheckSize2( state, 1 ) );
	unify::Size< float > r( CheckSize2( state, 2 ) );
	unify::Size< float > result( l / r );

	PushSize2( state, result );
	return 1;
}
 
int Size2_Units( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::Size< float > size3( CheckSize2( state, 1 ) );

	lua_pushnumber( state, size3.Units() );
	return 1;
}

static const luaL_Reg SizeFunctions[] =
{
	{ "New", Size2_New },
	{ "Zero", Size2_Zero },

	{ "Add", Size2_Add },
	{ "Sub", Size2_Sub },
	{ "Mul", Size2_Mul },
	{ "Div", Size2_Div },

	{ "Units", Size2_Units },

	{ "ToString", Size2_ToString },
	{ nullptr, nullptr }
};

void RegisterSize2( lua_State * state )
{
	luaL_newlib( state, SizeFunctions );
	lua_setglobal( state, "Size2" );
}
