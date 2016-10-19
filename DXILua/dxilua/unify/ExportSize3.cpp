// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/unify/ExportSize3.h>
#include <dxilua/DXILua.h>

using namespace dxilua;
using namespace dxi;

int Size3_New( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	float width = (float)lua_tonumber( state, 1 );
	float height = (float)lua_tonumber( state, 2 );
	float depth = (float)lua_tonumber( state, 3 );

	PushSize3( state, unify::Size3< float >( width, height, depth ) );

	return 1;
}

int Size3_Zero( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	PushSize3( state, unify::Size3< float >( 0, 0, 0 ) );

	return 1;
}

int Size3_ToString( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::Size3< float > size3( CheckSize3( state, 1 ) );

	lua_pushstring( state, size3.ToString().c_str() );
	return 1;
}

int Size3_Add( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::Size3< float > l( CheckSize3( state, 1 ) );
	unify::Size3< float > r( CheckSize3( state, 2 ) );
	unify::Size3< float > result( l + r );

	PushSize3( state, result );
	return 1;
}

int Size3_Sub( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::Size3< float > l( CheckSize3( state, 1 ) );
	unify::Size3< float > r( CheckSize3( state, 2 ) );
	unify::Size3< float > result( l - r );

	PushSize3( state, result );
	return 1;
}

int Size3_Mul( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::Size3< float > l( CheckSize3( state, 1 ) );
	unify::Size3< float > r( CheckSize3( state, 2 ) );
	unify::Size3< float > result( l * r );

	PushSize3( state, result );
	return 1;
}

int Size3_Div( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::Size3< float > l( CheckSize3( state, 1 ) );
	unify::Size3< float > r( CheckSize3( state, 2 ) );
	unify::Size3< float > result( l / r );

	PushSize3( state, result );
	return 1;
}
 
int Size3_Units( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::Size3< float > size3( CheckSize3( state, 1 ) );

	lua_pushnumber( state, size3.Units() );
	return 1;
}

static const luaL_Reg Size3Functions[] =
{
	{ "New", Size3_New },
	{ "Zero", Size3_Zero },

	{ "Add", Size3_Add },
	{ "Sub", Size3_Sub },
	{ "Mul", Size3_Mul },
	{ "Div", Size3_Div },

	{ "Units", Size3_Units },

	{ "ToString", Size3_ToString },
	{ nullptr, nullptr }
};

void RegisterSize3( lua_State * state )
{
	luaL_newlib( state, Size3Functions );
	lua_setglobal( state, "Size3" );
}
