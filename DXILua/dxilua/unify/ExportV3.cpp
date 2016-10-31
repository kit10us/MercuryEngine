// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/unify/ExportV3.h>
#include <dxilua/ScriptEngine.h>

#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/unify/ExportColor.h>
#include <dxilua/unify/ExportSize2.h>
#include <dxilua/unify/ExportSize2.h>
#include <dxilua/unify/ExportV2.h>
#include <dxilua/unify/ExportV3.h>

using namespace dxilua;
using namespace dxi;
 
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

int V3_New( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	float x = (float)lua_tonumber( state, 1 );
	float y = (float)lua_tonumber( state, 2 );
	float z = (float)lua_tonumber( state, 3 );

	PushV3( state, unify::V3< float >( x, y, z ) );

	return 1;
}

int V3_Zero( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	PushV3( state, unify::V3< float >( 0, 0, 0 ) );

	return 1;
}

int V3_ToString( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::V3< float > v3( CheckV3( state, 1 ) );

	lua_pushstring( state, v3.ToString().c_str() );
	return 1;
}

int V3_Add( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( CheckV3( state, 1 ) );
	unify::V3< float > r( CheckV3( state, 2 ) );
	unify::V3< float > result( l + r );

	PushV3( state, result );
	return 1;
}

int V3_Sub( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( CheckV3( state, 1 ) );
	unify::V3< float > r( CheckV3( state, 2 ) );
	unify::V3< float > result( l - r );

	PushV3( state, result );
	return 1;
}

int V3_Mul( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( CheckV3( state, 1 ) );
	unify::V3< float > r( CheckV3( state, 2 ) );
	unify::V3< float > result( l * r );

	PushV3( state, result );
	return 1;
}

int V3_Div( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( CheckV3( state, 1 ) );
	unify::V3< float > r( CheckV3( state, 2 ) );
	unify::V3< float > result( l / r );

	PushV3( state, result );
	return 1;
}
 
int V3_Length( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::V3< float > v3( CheckV3( state, 1 ) );

	lua_pushnumber( state, v3.Length() );
	return 1;
}

int V3_Normalize( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::V3< float > v3( CheckV3( state, 1 ) );

	PushV3( state, unify::V3< float >::V3Normalized( v3 ) );
	return 1;
}			   

int V3_DistanceTo( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( CheckV3( state, 1 ) );
	unify::V3< float > r( CheckV3( state, 2 ) );

	lua_pushnumber( state, l.DistanceTo( r ) );
	return 1;
}

int V3_Dot( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( CheckV3( state, 1 ) );
	unify::V3< float > r( CheckV3( state, 2 ) );

	lua_pushnumber( state, l.Dot( r ) );
	return 1;
}

int V3_Cross( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( CheckV3( state, 1 ) );
	unify::V3< float > r( CheckV3( state, 2 ) );
	unify::V3< float > result( unify::V3< float >::V3Cross( l,  r ) );

	PushV3( state, result );
	return 1;
}

int V3_Inverse( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::V3< float > v3( CheckV3( state, 1 ) );

	PushV3( state, unify::V3< float >::V3Inverse( v3 ) );
	return 1;
}

int V3_Lerp( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	unify::V3< float > l( CheckV3( state, 1 ) );
	unify::V3< float > r( CheckV3( state, 2 ) );
	float delta = (float)lua_tonumber( state, 3 );
	unify::V3< float > result( unify::V3< float >::V3Lerp( l, r, delta ) );

	PushV3( state, result );
	return 1;
}

static const luaL_Reg V3Functions[] =
{
	{ "New", V3_New },
	{ "Zero", V3_Zero },

	{ "Add", V3_Add },
	{ "Sub", V3_Sub },
	{ "Mul", V3_Mul },
	{ "Div", V3_Div },

	{ "Length", V3_Length },
	{ "Normalize", V3_Normalize },
	{ "DistanceTo", V3_DistanceTo },
	{ "Dot", V3_Dot },
	{ "Cross", V3_Cross },
	{ "Inverse", V3_Inverse },

	{ "ToString", V3_ToString },
	{ nullptr, nullptr }
};

void RegisterV3( lua_State * state )
{
	luaL_newlib( state, V3Functions );
	lua_setglobal( state, "V3" );
}
