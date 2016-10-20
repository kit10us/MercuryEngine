// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/unify/ExportTexCoords.h>
#include <dxilua/DXILua.h>

#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/unify/ExportColor.h>
#include <dxilua/unify/ExportSize2.h>
#include <dxilua/unify/ExportSize3.h>
#include <dxilua/unify/ExportTexCoords.h>
#include <dxilua/unify/ExportV3.h>


using namespace dxilua;
using namespace dxi;

unify::TexCoords CheckTexCoords( lua_State * state, int index )
{
	luaL_checktype( state, index, LUA_TTABLE );

	lua_getfield( state, index, "u" );
	lua_getfield( state, index, "v" );

	float u = (float)luaL_checknumber( state, -2 );
	float v = (float)luaL_checknumber( state, -1 );

	lua_pop( state, 2 );

	return unify::TexCoords( u, v );
}

int PushTexCoords( lua_State * state, unify::TexCoords TexCoords )
{
	lua_newtable( state ); // Create table.

	lua_pushstring( state, "u" );
	lua_pushnumber( state, TexCoords.u );
	lua_settable( state, -3 );

	lua_pushstring( state, "v" );
	lua_pushnumber( state, TexCoords.v );
	lua_settable( state, -3 );

	return 1;
}

int TexCoordsNew( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	float u = (float)lua_tonumber( state, 1 );
	float v = (float)lua_tonumber( state, 2 );

	PushTexCoords( state, unify::TexCoords( u, v ) );

	return 1;
}

int TexCoordsZero( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	PushTexCoords( state, unify::TexCoords( 0, 0 ) );

	return 1;
}

int TexCoordsToString( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::TexCoords TexCoords( CheckTexCoords( state, 1 ) );

	lua_pushstring( state, TexCoords.ToString().c_str() );
	return 1;
}

int TexCoordsAdd( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::TexCoords l( CheckTexCoords( state, 1 ) );
	unify::TexCoords r( CheckTexCoords( state, 2 ) );
	unify::TexCoords result( l + r );

	PushTexCoords( state, result );
	return 1;
}

int TexCoordsSub( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::TexCoords l( CheckTexCoords( state, 1 ) );
	unify::TexCoords r( CheckTexCoords( state, 2 ) );
	unify::TexCoords result( l - r );

	PushTexCoords( state, result );
	return 1;
}

int TexCoordsMul( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::TexCoords l( CheckTexCoords( state, 1 ) );
	unify::TexCoords r( CheckTexCoords( state, 2 ) );
	unify::TexCoords result( l * r );

	PushTexCoords( state, result );
	return 1;
}

int TexCoordsDiv( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::TexCoords l( CheckTexCoords( state, 1 ) );
	unify::TexCoords r( CheckTexCoords( state, 2 ) );
	unify::TexCoords result( l / r );

	PushTexCoords( state, result );
	return 1;
}
 
static const luaL_Reg TexCoordsFuncs[] =
{
	{ "New", TexCoordsNew },
	{ "Zero", TexCoordsZero },

	{ "Add", TexCoordsAdd },
	{ "Sub", TexCoordsSub },
	{ "Mul", TexCoordsMul },
	{ "Div", TexCoordsDiv },

	{ "ToString", TexCoordsToString },
	{ nullptr, nullptr }
};

int ExportTexCoords( lua_State * state )
{
	luaL_newlib( state, TexCoordsFuncs );
	lua_setglobal( state, "TexCoords" );
	return 1;
}
