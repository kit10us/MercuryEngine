// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/unify/ExportV2.h>
#include <dxilua/DXILua.h>

#include <dxilua/Matrix.h>
#include <dxilua/Color.h>
#include <dxilua/Size2.h>
#include <dxilua/Size3.h>
#include <dxilua/V2.h>
#include <dxilua/V3.h>


using namespace dxilua;
using namespace dxi;

int V2New( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	float x = (float)lua_tonumber( state, 1 );
	float y = (float)lua_tonumber( state, 2 );

	PushV2( state, unify::V2< float >( x, y ) );

	return 1;
}

int V2Zero( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	PushV2( state, unify::V2< float >( 0, 0 ) );

	return 1;
}

int V2ToString( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::V2< float > V2( CheckV2( state, 1 ) );

	lua_pushstring( state, V2.ToString().c_str() );
	return 1;
}

int V2Add( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V2< float > l( CheckV2( state, 1 ) );
	unify::V2< float > r( CheckV2( state, 2 ) );
	unify::V2< float > result( l + r );

	PushV2( state, result );
	return 1;
}

int V2Sub( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V2< float > l( CheckV2( state, 1 ) );
	unify::V2< float > r( CheckV2( state, 2 ) );
	unify::V2< float > result( l - r );

	PushV2( state, result );
	return 1;
}

int V2Mul( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V2< float > l( CheckV2( state, 1 ) );
	unify::V2< float > r( CheckV2( state, 2 ) );
	unify::V2< float > result( l * r );

	PushV2( state, result );
	return 1;
}

int V2Div( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V2< float > l( CheckV2( state, 1 ) );
	unify::V2< float > r( CheckV2( state, 2 ) );
	unify::V2< float > result( l / r );

	PushV2( state, result );
	return 1;
}
 
int V2Length( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::V2< float > V2( CheckV2( state, 1 ) );

	lua_pushnumber( state, V2.Length() );
	return 1;
}

static const luaL_Reg V2Funcs[] =
{
	{ "New", V2New },
	{ "Zero", V2Zero },

	{ "Add", V2Add },
	{ "Sub", V2Sub },
	{ "Mul", V2Mul },
	{ "Div", V2Div },

	{ "Length", V2Length },

	{ "ToString", V2ToString },
	{ nullptr, nullptr }
};

int ExportV2( lua_State * state )
{
	luaL_newlib( state, V2Funcs );
	lua_setglobal( state, "V2" );
	return 1;
}
