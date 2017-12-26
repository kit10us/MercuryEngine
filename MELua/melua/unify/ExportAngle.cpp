// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/unify/ExportV3.h>
#include <melua/ScriptEngine.h>

#include <melua/unify/ExportMatrix.h>
#include <melua/unify/ExportColor.h>
#include <melua/unify/ExportSize2.h>
#include <melua/unify/ExportSize2.h>
#include <melua/unify/ExportV2.h>
#include <melua/unify/ExportV3.h>

using namespace melua;

int Angle_Degrees( lua_State * state )
{
	auto * se = ScriptEngine::GetInstance();
	se->AssertTop( 1 );

	float degrees = (float)lua_tonumber( state, 1 );
	float angle = degrees * (3.14159265f / 180.0f);
	lua_pushnumber( state, angle );

	return 1;
}

int Angle_Radians( lua_State * state )
{
	auto * se = ScriptEngine::GetInstance();
	se->AssertTop( 1 );

	float radians = (float)lua_tonumber( state, 1 );
	float angle = radians;
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
