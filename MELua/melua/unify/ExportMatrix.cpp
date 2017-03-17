// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <melua/unify/ExportMatrix.h>
#include <melua/unify/ExportV3.h>
#include <melua/ScriptEngine.h>

#include <melua/unify/ExportMatrix.h>
#include <melua/unify/ExportColor.h>
#include <melua/unify/ExportSize2.h>
#include <melua/unify/ExportSize3.h>
#include <melua/unify/ExportV2.h>
#include <melua/unify/ExportV3.h>
					 
using namespace melua;
using namespace me;

unify::Matrix CheckMatrix( lua_State* state, int index )
{
	luaL_checktype( state, index, LUA_TTABLE );

	unify::Matrix mat;
	int i = 1;
	for ( int c = 0; c < 4; ++c )
	{
		for ( int r = 0; r < 4; ++r )
		{
			lua_rawgeti( state, index, i++ );
			float value = (float)lua_tonumber( state, -1 );
			mat.Set( r, c, value );
		}
	}
	return mat;
}

int PushMatrix( lua_State* state, const unify::Matrix & mat )
{
	lua_newtable( state );

	int index = 1;
	for ( int c = 0; c < 4; ++c )
	{
		for ( int r = 0; r < 4; ++r )
		{
			double value = mat( r, c );
			lua_pushnumber( state, value );
			lua_rawseti( state, -2, index++ );
		}
	}

	return 1;
}

int Matrix_NewIdentity( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	lua_newtable( state );

	unify::Matrix mat = unify::MatrixIdentity();
	PushMatrix( state, mat );

	return 1;
}

int Matrix_NewZero( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	lua_newtable( state );

	unify::Matrix mat = unify::MatrixZero();
	PushMatrix( state, mat );

	return 1;
}

int Matrix_NewTranslate( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	lua_newtable( state );

	unify::V3< float > vector( CheckV3( state, 1 ) );

	unify::Matrix mat = unify::MatrixTranslate( vector );
	PushMatrix( state, mat );

	return 1;
}

int Matrix_NewOrthoOffCenterLH( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 6 );

	float left = (float)lua_tonumber( state, 1 );
	float right = (float)lua_tonumber( state, 2 );
	float bottom = (float)lua_tonumber( state, 3 );
	float top = (float)lua_tonumber( state, 4 );
	float zn = (float)lua_tonumber( state, 5 );
	float zf = (float)lua_tonumber( state, 6 );

	lua_newtable( state );

	unify::Matrix mat = unify::MatrixOrthoOffCenterLH( left, right, bottom, top, zn, zf );
	PushMatrix( state, mat );

	return 1;
}

int Matrix_NewScale( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	lua_newtable( state );

	unify::Matrix mat;

	int type = lua_type( state, 1 );
	if ( type == LUA_TTABLE ) // V3
	{
		unify::V3< float > scale( CheckV3( state, 1 ) );
		mat = unify::MatrixScale( scale );
	}
	else
	{
		float scale = (float)lua_tonumber( state, 1 );
		mat = unify::MatrixScale( scale );
	}

	PushMatrix( state, mat );

	return 1;
}

int Matrix_NewRotationAboutAxis( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	lua_newtable( state );

	unify::V3< float > axis( CheckV3( state, 1 ) );
	float angle = (float)lua_tonumber( state, 2 );

	unify::Matrix mat = unify::MatrixRotationAboutAxis( axis, unify::AngleInRadians( angle ) );
	PushMatrix( state, mat );

	return 1;
}

int Matrix_NewPerspectiveFovLH( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	float w = (float)lua_tonumber( state, 1 );
	float h = (float)lua_tonumber( state, 2 );
	float zn = (float)lua_tonumber( state, 3 );
	float zf = (float)lua_tonumber( state, 4 );

	lua_newtable( state );

	unify::Matrix mat = unify::MatrixPerspectiveFovLH( w, h, zn, zf );
	PushMatrix( state, mat );

	return 1;
}

int Matrix_NewRotationX( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	lua_newtable( state );

	float angle = (float)lua_tonumber( state, 1 );

	unify::Matrix mat = unify::MatrixRotationX( unify::AngleInRadians( angle ) );
	PushMatrix( state, mat );

	return 1;
}

int Matrix_NewRotationY( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	lua_newtable( state );

	float angle = (float)lua_tonumber( state, 1 );

	unify::Matrix mat = unify::MatrixRotationY( unify::AngleInRadians( angle ) );
	PushMatrix( state, mat );

	return 1;
}
int Matrix_NewRotationZ( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	lua_newtable( state );

	float angle = (float)lua_tonumber( state, 1 );

	unify::Matrix mat = unify::MatrixRotationZ( unify::AngleInRadians( angle ) );
	PushMatrix( state, mat );

	return 1;
}

int Matrix_NewLookAtLH( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	lua_newtable( state );

	unify::V3< float > eye( CheckV3( state, 1 ) );
	unify::V3< float > at( CheckV3( state, 2 ) );
	unify::V3< float > up( CheckV3( state, 3 ) );

	unify::Matrix mat = unify::MatrixLookAtLH( eye, at, up );
	PushMatrix( state, mat );

	return 1;
}

int Matrix_Add( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	lua_newtable( state );

	unify::Matrix l = CheckMatrix( state, 1 );
	unify::Matrix r = CheckMatrix( state, 2 );
	unify::Matrix mat( l + r );

	PushMatrix( state, mat );

	return 1;
}

int Matrix_Sub( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	lua_newtable( state );

	unify::Matrix l = CheckMatrix( state, 1 );
	unify::Matrix r = CheckMatrix( state, 2 );
	unify::Matrix mat( l - r );

	PushMatrix( state, mat );

	return 1;
}

int Matrix_Mul( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	lua_newtable( state );

	unify::Matrix l = CheckMatrix( state, 1 );

	unify::Matrix mat;

	if ( lua_type( state, 2 ) == LUA_TNUMBER )
	{
		float r = (float)lua_tonumber( state, 2 );
		mat = l * r;							  
	}
	else
	{
		unify::Matrix r = CheckMatrix( state, 2 );
		mat = l * r;
	}

	PushMatrix( state, mat );

	return 1;
}

int Matrix_Div( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	lua_newtable( state );

	unify::Matrix l = CheckMatrix( state, 1 );
	float r = (float)lua_tonumber( state, 2 );
	unify::Matrix mat( l / r );

	PushMatrix( state, mat );

	return 1;
}

static const luaL_Reg matrixFuncs[] =
{
	{ "NewIdentity", Matrix_NewIdentity },
	{ "NewZero", Matrix_NewZero },
	{ "NewTranslate", Matrix_NewTranslate },
	{ "NewOrthoOffCenterLH", Matrix_NewOrthoOffCenterLH },
	{ "NewScale", Matrix_NewScale },
	{ "NewRotationAboutAxis", Matrix_NewRotationAboutAxis },
	{ "NewPerspectiveFovLH", Matrix_NewPerspectiveFovLH },
	{ "NewRotationX", Matrix_NewRotationX },
	{ "NewRotationY", Matrix_NewRotationY },
	{ "NewRotationZ", Matrix_NewRotationZ },
	{ "NewLookAtLH", Matrix_NewLookAtLH },
	{ "Add", Matrix_Add },
	{ "Sub", Matrix_Sub },
	{ "Mul", Matrix_Mul },
	{ "Div", Matrix_Div },
	{ nullptr, nullptr }
};

int ExportMatrix( lua_State * state )
{
	luaL_newlib( state, matrixFuncs );
	lua_setglobal( state, "Matrix" );
	return 1;
}