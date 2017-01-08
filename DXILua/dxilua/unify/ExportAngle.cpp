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

int Angle_Degrees( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	float angle = (float)lua_tonumber( state, 1 ) * (3.14159265f / 180.0f);
	lua_pushnumber( state, angle );

	return 1;
}

int Angle_Radians( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	float angle = (float)lua_tonumber( state, 1 );
	lua_pushnumber( state, angle );

	return 1;
}

static const luaL_Reg AngleFunctions[] =
{
	{ "Degrees", Angle_Degrees },
	{ "Radians", Angle_Radians },

	{ nullptr, nullptr }
};

void RegisterAngle( lua_State * state )
{
	luaL_newlib( state, AngleFunctions );
	lua_setglobal( state, "Angle" );
}
