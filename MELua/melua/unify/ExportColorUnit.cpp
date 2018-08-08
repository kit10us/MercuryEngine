// Copyright (c) 2002 - 2011, Evil Quail LLC
// All Rights Reserved

#include <melua/unify/ExportColor.h>
#include <melua/MELua.h>

#include <melua/unify/ExportMatrix.h>
#include <melua/unify/ExportColor.h>
#include <melua/unify/ExportSize2.h>
#include <melua/unify/ExportSize3.h>
#include <melua/unify/ExportV2.h>
#include <melua/unify/ExportV3.h>

using namespace melua;

int PushColor( lua_State * state, unify::ColorUnit color )
{
	lua_newtable( state ); // Create table.

	lua_pushstring( state, "r" );
	lua_pushnumber( state, color.r );
	lua_settable( state, -3 );

	lua_pushstring( state, "g" );
	lua_pushnumber( state, color.g );
	lua_settable( state, -3 );

	lua_pushstring( state, "b" );
	lua_pushnumber( state, color.b );
	lua_settable( state, -3 );

	lua_pushstring( state, "a" );
	lua_pushnumber( state, color.a );
	lua_settable( state, -3 );
	return 1;
}

unify::ColorUnit CheckColor( lua_State * state, int index )
{
	luaL_checktype( state, index, LUA_TTABLE );

	lua_getfield( state, index, "r" );
	float r = (float)luaL_checknumber( state, -1 );

	lua_getfield( state, index, "g" );
	float g = (float)luaL_checknumber( state, -1 );

	lua_getfield( state, index, "b" );
	float b = (float)luaL_checknumber( state, -1 );

	lua_getfield( state, index, "a" );
	float a = (float)luaL_checknumber( state, -1 );

	lua_pop( state, 4 );

	return unify::ColorUnit::ColorUnitRGBA( r, g, b, a );
}

int Color_NewRGBA( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	float r = (float)lua_tonumber( state, 1 );
	float g = (float)lua_tonumber( state, 2 );
	float b = (float)lua_tonumber( state, 3 );
	float a = (float)lua_tonumber( state, 4 );

	PushColor( state, unify::ColorUnit::ColorUnitRGBA( r, g, b, a ) );

	return 1;
}

int Color_NewARGB( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	float a = (float)lua_tonumber( state, 1 );
	float r = (float)lua_tonumber( state, 2 );
	float g = (float)lua_tonumber( state, 3 );
	float b = (float)lua_tonumber( state, 4 );

	PushColor( state, unify::ColorUnit::ColorUnitARGB( a, r, g, b ) );

	return 1;
}

int Color_NewRGB( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	float r = (float)lua_tonumber( state, 1 );
	float g = (float)lua_tonumber( state, 2 );
	float b = (float)lua_tonumber( state, 3 );

	PushColor( state, unify::ColorUnit::ColorUnitRGB( r, g, b ) );

	return 1;
}

int Color_NewWhite( lua_State * state )
{
	int args = lua_gettop( state );
	float a = 1.0f;
	if ( args == 1 )
	{
		float a = (float)lua_tonumber( state, 1 );
	}

	PushColor( state, unify::ColorUnit::ColorUnitWhite( a ) );

	return 1;
}

int Color_NewRed( lua_State * state )
{
	int args = lua_gettop( state );
	float r = 1.0f;
	float a = 1.0f;
	if ( args > 1 )
	{
		float r = (float)lua_tonumber( state, 1 );
	}
	if ( args > 2 )
	{
		float a = (float)lua_tonumber( state, 2 );
	}

	PushColor( state, unify::ColorUnit::ColorUnitRGBA( r, 0.0f, 0.0f, a ) );

	return 1;
}

int Color_NewGreen( lua_State * state )
{
	int args = lua_gettop( state );
	float g = 1.0f;
	float a = 1.0f;
	if ( args > 1 )
	{
		float g = (float)lua_tonumber( state, 1 );
	}
	if ( args > 2 )
	{
		float a = (float)lua_tonumber( state, 2 );
	}

	PushColor( state, unify::ColorUnit::ColorUnitGreen( g, a ) );

	return 1;
}

int Color_NewBlue( lua_State * state )
{
	int args = lua_gettop( state );
	float b = 1.0f;
	float a = 1.0f;
	if ( args > 1 )
	{
		float b = (float)lua_tonumber( state, 1 );
	}
	if ( args > 2 )
	{
		float a = (float)lua_tonumber( state, 2 );
	}

	PushColor( state, unify::ColorUnit::ColorUnitRGBA( 0.0f, 0.0f, b, a ) );

	return 1;
}

int Color_NewGrey( lua_State * state )
{
	int args = lua_gettop( state );
	float grey = 1.0f;
	float a = 1.0f;
	if ( args > 1 )
	{
		float grey = (float)lua_tonumber( state, 1 );
	}
	if ( args > 2 )
	{
		float a = (float)lua_tonumber( state, 2 );
	}

	PushColor( state, unify::ColorUnit::ColorUnitGrey( grey, a ) );

	return 1;
}

int Color_NewBlack( lua_State * state )
{
	int args = lua_gettop( state );
	float a = 1.0f;
	if ( args > 1 )
	{
		float a = (float)lua_tonumber( state, 1 );
	}

	PushColor( state, unify::ColorUnit::ColorUnitBlack( a ) );

	return 1;
}

int Color_NewZero( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	PushColor( state, unify::ColorUnit::ColorUnitZero() );

	return 1;
}

int Color_ToString( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::ColorUnit color( CheckColor( state, 1 ) );

	lua_pushstring( state, color.ToString().c_str() );
	return 1;
}

int Color_Add( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::ColorUnit l( CheckColor( state, 1 ) );
	unify::ColorUnit r( CheckColor( state, 2 ) );
	unify::ColorUnit result( l + r );

	PushColor( state, result );
	return 1;
}

int Color_Sub( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::ColorUnit l( CheckColor( state, 1 ) );
	unify::ColorUnit r( CheckColor( state, 2 ) );
	unify::ColorUnit result( l - r );

	PushColor( state, result );
	return 1;
}

int Color_Mul( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::ColorUnit l( CheckColor( state, 1 ) );
	unify::ColorUnit r( CheckColor( state, 2 ) );
	unify::ColorUnit result( l * r );

	PushColor( state, result );
	return 1;
}

int Color_Div( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::ColorUnit l( CheckColor( state, 1 ) );
	unify::ColorUnit r( CheckColor( state, 2 ) );
	unify::ColorUnit result( l / r );

	PushColor( state, result );
	return 1;
}

int Color_Normalize( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::ColorUnit color( CheckColor( state, 1 ) );

	PushColor( state, color.Normalize() );
	return 1;
}			   

int Color_Lerp( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	unify::ColorUnit l( CheckColor( state, 1 ) );
	unify::ColorUnit r( CheckColor( state, 2 ) );
	float delta = (float)lua_tonumber( state, 3 );
	unify::ColorUnit result( unify::ColorUnit::ColorUnitLerp( l, r, delta ) );

	PushColor( state, result );
	return 1;
}

static const luaL_Reg ColorFunctions[] =
{
	{ "NewRGBA", Color_NewRGBA },
	{ "NewARGB", Color_NewARGB },
	{ "NewRGB", Color_NewRGB },
	{ "NewWhite", Color_NewWhite },
	{ "NewRed", Color_NewRed },
	{ "NewGreen", Color_NewGreen },
	{ "NewBlue", Color_NewBlue },
	{ "NewGrey", Color_NewGrey },
	{ "NewBlack", Color_NewBlack },
	{ "NewZero", Color_NewZero },
	{ "ToString", Color_ToString },

	{ "Add", Color_Add },
	{ "Sub", Color_Sub },
	{ "Mul", Color_Mul },
	{ "Div", Color_Div },

	{ "Normalize", Color_Normalize },
	{ "Lerp", Color_Lerp },

	{ nullptr, nullptr }
};

void RegisterColor( lua_State * state )
{
	luaL_newlib( state, ColorFunctions );
	lua_setglobal( state, "Color" );
}
