// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/unify/ExportV3.h>
#include <dxilua/DXILua.h>

#include <dxilua/Matrix.h>
#include <dxilua/Color.h>
#include <dxilua/Size2.h>
#include <dxilua/Size2.h>
#include <dxilua/V2.h>
#include <dxilua/V3.h>

using namespace dxilua;
using namespace dxi;

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
