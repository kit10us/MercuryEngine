// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ExportScenes.h>
#include <dxilua/DXILua.h>

using namespace dxilua;
using namespace dxi;

// Helper functions...

unify::V3< float > lua_ToV3( lua_State * state, int index )
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

int lua_PushV3( lua_State * state, unify::V3< float > v3 )
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

// Member functions...

int V3New( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	float x = (float)lua_tonumber( state, 1 );
	float y = (float)lua_tonumber( state, 2 );
	float z = (float)lua_tonumber( state, 3 );

	lua_PushV3( state, unify::V3< float >( x, y, z ) );

	return 1;
}

int V3Zero( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	lua_PushV3( state, unify::V3< float >( 0, 0, 0 ) );

	return 1;
}

int V3ToString( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::V3< float > v3( lua_ToV3( state, 1 ) );

	lua_pushstring( state, v3.ToString().c_str() );
	return 1;
}

int V3Add( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( lua_ToV3( state, 1 ) );
	unify::V3< float > r( lua_ToV3( state, 2 ) );
	unify::V3< float > result( l + r );

	lua_PushV3( state, result );
	return 1;
}

int V3Sub( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( lua_ToV3( state, 1 ) );
	unify::V3< float > r( lua_ToV3( state, 2 ) );
	unify::V3< float > result( l - r );

	lua_PushV3( state, result );
	return 1;
}

int V3Mul( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( lua_ToV3( state, 1 ) );
	unify::V3< float > r( lua_ToV3( state, 2 ) );
	unify::V3< float > result( l * r );

	lua_PushV3( state, result );
	return 1;
}

int V3Div( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( lua_ToV3( state, 1 ) );
	unify::V3< float > r( lua_ToV3( state, 2 ) );
	unify::V3< float > result( l / r );

	lua_PushV3( state, result );
	return 1;
}
 
int V3Length( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::V3< float > v3( lua_ToV3( state, 1 ) );

	lua_pushnumber( state, v3.Length() );
	return 1;
}

int V3Normalize( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::V3< float > v3( lua_ToV3( state, 1 ) );

	lua_PushV3( state, unify::V3< float >::V3Normalized( v3 ) );
	return 1;
}			   

int V3DistanceTo( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( lua_ToV3( state, 1 ) );
	unify::V3< float > r( lua_ToV3( state, 2 ) );

	lua_pushnumber( state, l.DistanceTo( r ) );
	return 1;
}

int V3Dot( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( lua_ToV3( state, 1 ) );
	unify::V3< float > r( lua_ToV3( state, 2 ) );

	lua_pushnumber( state, l.Dot( r ) );
	return 1;
}

int V3Cross( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( lua_ToV3( state, 1 ) );
	unify::V3< float > r( lua_ToV3( state, 2 ) );
	unify::V3< float > result( unify::V3< float >::V3Cross( l,  r ) );

	lua_PushV3( state, result );
	return 1;
}

int V3Inverse( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::V3< float > v3( lua_ToV3( state, 1 ) );

	lua_PushV3( state, unify::V3< float >::V3Inverse( v3 ) );
	return 1;
}

static const luaL_Reg v3Funcs[] =
{
	{ "New", V3New },
	{ "Zero", V3Zero },

	{ "Add", V3Add },
	{ "Sub", V3Sub },
	{ "Mul", V3Mul },
	{ "Div", V3Div },

	{ "Length", V3Length },
	{ "Normalize", V3Normalize },
	{ "DistanceTo", V3DistanceTo },
	{ "Dot", V3Dot },
	{ "Cross", V3Cross },
	{ "Inverse", V3Inverse },

	{ "ToString", V3ToString },
	{ nullptr, nullptr }
};

int ExportV3( lua_State * state )
{
	luaL_newlib( state, v3Funcs );
	lua_setglobal( state, "V3" );
	return 1;
}
