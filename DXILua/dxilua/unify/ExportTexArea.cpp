// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/unify/ExportTexArea.h>
#include <dxilua/ScriptEngine.h>

#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/unify/ExportColor.h>
#include <dxilua/unify/ExportSize2.h>
#include <dxilua/unify/ExportSize3.h>
#include <dxilua/unify/ExportTexCoords.h>
#include <dxilua/unify/ExportTexArea.h>
#include <dxilua/unify/ExportV3.h>

using namespace dxilua;
using namespace me;

unify::TexArea CheckTexArea( lua_State * state, int index )
{
	luaL_checktype( state, index, LUA_TTABLE );

	lua_getfield( state, index, "ulu" );
	lua_getfield( state, index, "ulv" );
	lua_getfield( state, index, "dru" );
	lua_getfield( state, index, "drv" );

	float ulu = (float)luaL_checknumber( state, -4 );
	float ulv = (float)luaL_checknumber( state, -3 );
	float dru = (float)luaL_checknumber( state, -2 );
	float drv = (float)luaL_checknumber( state, -1 );

	lua_pop( state, 5 );

	return unify::TexArea( ulu, ulu, dru, drv );
}

int PushTexArea( lua_State * state, unify::TexArea area )
{
	lua_newtable( state ); // Create table.

	lua_pushstring( state, "ulu" );
	lua_pushnumber( state, area.ul.u );
	lua_settable( state, -3 );

	lua_pushstring( state, "ulv" );
	lua_pushnumber( state, area.ul.v );
	lua_settable( state, -3 );

	lua_pushstring( state, "dru" );
	lua_pushnumber( state, area.dr.u );
	lua_settable( state, -3 );

	lua_pushstring( state, "drv" );
	lua_pushnumber( state, area.dr.v );
	lua_settable( state, -3 );

	return 1;
}

int TexArea_New( lua_State * state )
{
	int args = lua_gettop( state );
	
	if ( args == 2 )
	{
		unify::TexCoords ul = CheckTexCoords( state, 1 );
		unify::TexCoords dr = CheckTexCoords( state, 2 );
		PushTexArea( state, unify::TexArea( ul, dr ) );
	}
	else if ( args == 4 )
	{	
		float ul_u = (float)lua_tonumber( state, 1 );
		float ul_v = (float)lua_tonumber( state, 2 );
		float dr_u = (float)lua_tonumber( state, 3 );
		float dr_v = (float)lua_tonumber( state, 4 );

		PushTexArea( state, unify::TexArea( ul_u, ul_v, dr_u, dr_v ) );
	}
	else
	{
		lua_pushnil( state );
	}

	return 1;
}

int TexArea_NewOne( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	PushTexArea( state, unify::TexAreaOne() );

	return 1;
}

int TexArea_NewZero( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	PushTexArea( state, unify::TexAreaZero() );

	return 1;
}

int TexArea_NewFull( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	PushTexArea( state, unify::TexAreaFull() );

	return 1;
}
									
int TexArea_NewDouble( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	PushTexArea( state, unify::TexAreaDouble() );

	return 1;
}

static const luaL_Reg TexAreaFuncs[] =
{
	{ "New",		TexArea_New },
	{ "NewOne",		TexArea_NewOne },
	{ "NewZero",	TexArea_NewZero },
	{ "NewFull",	TexArea_NewFull },
	{ "NewDouble",	TexArea_NewDouble },
	{ nullptr, nullptr }
};

int ExportTexArea( lua_State * state )
{
	luaL_newlib( state, TexAreaFuncs );
	lua_setglobal( state, "TexArea" );
	return 1;
}
