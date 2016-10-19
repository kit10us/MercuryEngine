// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/unify/ExportSize2.h>
#include <dxilua/DXILua.h>

#include <dxilua/Matrix.h>
#include <dxilua/Color.h>
#include <dxilua/Size2.h>
#include <dxilua/Size3.h>
#include <dxilua/V2.h>
#include <dxilua/V3.h>


using namespace dxilua;
using namespace dxi;

int Size2_New( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	float width = (float)lua_tonumber( state, 1 );
	float height = (float)lua_tonumber( state, 2 );

	PushSize2( state, unify::Size< float >( width, height ) );

	return 1;
}

int Size2_NewZero( lua_State * state )
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
	{ "NewZero", Size2_NewZero },

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
