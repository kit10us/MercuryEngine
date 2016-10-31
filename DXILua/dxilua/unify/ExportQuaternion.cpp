// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/unify/ExportQuaternion.h>
#include <dxilua/ScriptEngine.h>

#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/unify/ExportQuaternion.h>

using namespace dxilua;
using namespace dxi;
 
unify::Quaternion CheckQuaternion( lua_State * state, int index )
{
	luaL_checktype( state, index, LUA_TTABLE );

	lua_getfield( state, index, "x" );
	lua_getfield( state, index, "y" );
	lua_getfield( state, index, "z" );
	lua_getfield( state, index, "w" );

	float x = (float)luaL_checknumber( state, -4 );
	float y = (float)luaL_checknumber( state, -3 );
	float z = (float)luaL_checknumber( state, -2 );
	float w = (float)luaL_checknumber( state, -1 );

	lua_pop( state, 3 );

	return unify::Quaternion( x, y, z, w );
}

int PushQuaternion( lua_State * state, unify::Quaternion Quaternion )
{
	lua_newtable( state ); // Create table.

	lua_pushstring( state, "x" );
	lua_pushnumber( state, Quaternion.x );
	lua_settable( state, -3 );

	lua_pushstring( state, "y" );
	lua_pushnumber( state, Quaternion.y );
	lua_settable( state, -3 );

	lua_pushstring( state, "z" );
	lua_pushnumber( state, Quaternion.z );
	lua_settable( state, -3 );

	lua_pushstring( state, "w" );
	lua_pushnumber( state, Quaternion.w );
	lua_settable( state, -3 );
	return 1;
}

int Quaternion_New( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	float x = (float)lua_tonumber( state, 1 );
	float y = (float)lua_tonumber( state, 2 );
	float z = (float)lua_tonumber( state, 3 );
	float w = (float)lua_tonumber( state, 4 );

	PushQuaternion( state, unify::Quaternion( x, y, z, w ) );

	return 1;
}

int Quaternion_FromEuler( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	float pitch = (float)lua_tonumber( state, 1 );
	float roll = (float)lua_tonumber( state, 2 );
	float yaw = (float)lua_tonumber( state, 3 );

	PushQuaternion( state, unify::QuaternionFromEuler( unify::V3< float >( pitch, roll, yaw ) ) );

	return 1;
}

int Quaternion_Zero( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	PushQuaternion( state, unify::Quaternion( 0, 0, 0, 0 ) );

	return 1;
}

int Quaternion_Identity( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	PushQuaternion( state, unify::QuaternionIdentity() );

	return 1;
}

int Quaternion_ToString( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::Quaternion q( CheckQuaternion( state, 1 ) );

	lua_pushstring( state, q.ToString().c_str() );
	return 1;
}

int Quaternion_Add( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::Quaternion l( CheckQuaternion( state, 1 ) );
	unify::Quaternion r( CheckQuaternion( state, 2 ) );
	unify::Quaternion result( l + r );

	PushQuaternion( state, result );
	return 1;
}

int Quaternion_Sub( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::Quaternion l( CheckQuaternion( state, 1 ) );
	unify::Quaternion r( CheckQuaternion( state, 2 ) );
	unify::Quaternion result( l - r );

	PushQuaternion( state, result );
	return 1;
}

int Quaternion_Mul( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::Quaternion l( CheckQuaternion( state, 1 ) );
	unify::Quaternion r( CheckQuaternion( state, 2 ) );
	unify::Quaternion result( l * r );

	PushQuaternion( state, result );
	return 1;
}

int Quaternion_Conjugate( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::Quaternion q( CheckQuaternion( state, 1 ) );

	PushQuaternion( state, q.Conjugate() );
	return 1;
}

int Quaternion_Normalize( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::Quaternion q( CheckQuaternion( state, 1 ) );

	PushQuaternion( state, q.Normalize() );
	return 1;
}			   

int Quaternion_Slerp( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	unify::Quaternion l( CheckQuaternion( state, 1 ) );
	unify::Quaternion r( CheckQuaternion( state, 2 ) );
	float delta = (float)lua_tonumber( state, 3 );
	unify::Quaternion result( unify::QuaternionSlerp( l, r, delta ) );

	PushQuaternion( state, result );
	return 1;
}

static const luaL_Reg QuaternionFunctions[] =
{
	{ "New", Quaternion_New },
	{ "Identity", Quaternion_Identity },
	{ "Zero", Quaternion_Zero },

	{ "Add", Quaternion_Add },
	{ "Sub", Quaternion_Sub },
	{ "Mul", Quaternion_Mul },

	{ "Conjugate", Quaternion_Conjugate },
	{ "Normalize", Quaternion_Normalize },
	{ "Slerp", Quaternion_Slerp },

	{ "ToString", Quaternion_ToString },
	{ nullptr, nullptr }
};

void RegisterQuaternion( lua_State * state )
{
	luaL_newlib( state, QuaternionFunctions );
	lua_setglobal( state, "Quaternion" );
}
