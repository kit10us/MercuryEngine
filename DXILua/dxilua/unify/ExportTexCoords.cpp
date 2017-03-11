// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/unify/ExportTexCoords.h>
#include <dxilua/ScriptEngine.h>

#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/unify/ExportColor.h>
#include <dxilua/unify/ExportSize2.h>
#include <dxilua/unify/ExportSize3.h>
#include <dxilua/unify/ExportTexCoords.h>
#include <dxilua/unify/ExportV3.h>

using namespace dxilua;
using namespace me;

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

int PushTexCoords( lua_State * state, unify::TexCoords texCoords )
{
	lua_newtable( state ); // Create table.

	lua_pushstring( state, "u" );
	lua_pushnumber( state, texCoords.u );
	lua_settable( state, -3 );

	lua_pushstring( state, "v" );
	lua_pushnumber( state, texCoords.v );
	lua_settable( state, -3 );

	return 1;
}

int TexCoords_New( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	float u = (float)lua_tonumber( state, 1 );
	float v = (float)lua_tonumber( state, 2 );

	PushTexCoords( state, unify::TexCoords( u, v ) );

	return 1;
}

int TexCoords_NewZero( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	PushTexCoords( state, unify::TexCoords( 0, 0 ) );

	return 1;
}

int TexCoords_Add( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::TexCoords l( CheckTexCoords( state, 1 ) );
	unify::TexCoords r( CheckTexCoords( state, 2 ) );
	unify::TexCoords result( l + r );

	PushTexCoords( state, result );
	return 1;
}

int TexCoords_Sub( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::TexCoords l( CheckTexCoords( state, 1 ) );
	unify::TexCoords r( CheckTexCoords( state, 2 ) );
	unify::TexCoords result( l - r );

	PushTexCoords( state, result );
	return 1;
}

int TexCoords_Mul( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::TexCoords l( CheckTexCoords( state, 1 ) );
	unify::TexCoords r( CheckTexCoords( state, 2 ) );
	unify::TexCoords result( l * r );

	PushTexCoords( state, result );
	return 1;
}

int TexCoords_Div( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::TexCoords l( CheckTexCoords( state, 1 ) );
	unify::TexCoords r( CheckTexCoords( state, 2 ) );
	unify::TexCoords result( l / r );

	PushTexCoords( state, result );
	return 1;
}

int TexCoords_ToString( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::TexCoords TexCoords( CheckTexCoords( state, 1 ) );

	lua_pushstring( state, TexCoords.ToString().c_str() );
	return 1;
}
 
static const luaL_Reg TexCoordsFuncs[] =
{
	{ "New",		TexCoords_New },
	{ "NewZero",	TexCoords_NewZero },
	{ "Add",		TexCoords_Add },
	{ "Sub",		TexCoords_Sub },
	{ "Mul",		TexCoords_Mul },
	{ "Div",		TexCoords_Div },

	{ "ToString",	TexCoords_ToString },
	{ nullptr, nullptr }
};

int ExportTexCoords( lua_State * state )
{
	luaL_newlib( state, TexCoordsFuncs );
	lua_setglobal( state, "TexCoords" );
	return 1;
}
