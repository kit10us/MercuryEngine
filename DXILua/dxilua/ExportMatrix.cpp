// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ExportMatrix.h>
#include <dxilua/DXILua.h>

using namespace dxilua;
using namespace dxi;

unify::Matrix CheckMatrix( lua_State* state, int index )
{
	luaL_checktype( state, index, LUA_TTABLE );

	unify::Matrix mat;
	int i = 1;
	for( int c = 0; c < 4; ++c )
	{
		for( int r = 0; r < 4; ++r )
		{
			lua_rawgeti( state, index, i++ );
			float value = (float)lua_tonumber( state, -1 );
			mat.Set( r, c, value );
		}
	}
	return mat;
}


int Matrix_MakeIdentity( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	lua_newtable( state );

	unify::Matrix mat = unify::Matrix::MatrixIdentity();

	int index = 1;
	for( int c = 0; c < 4; ++c )
	{
		for( int r = 0; r < 4; ++r )
		{
			double value = mat( r, c );
			lua_pushnumber( state, value );
			lua_rawseti( state, -2, index++ );
		}
	}

	return 1;
}

extern "C"
int Matrix_MakePerspectiveFovLH( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	float w = (float)lua_tonumber( state, 1 );
	float h = (float)lua_tonumber( state, 2 );
	float zn = (float)lua_tonumber( state, 3 );
	float zf = (float)lua_tonumber( state, 4 );

	lua_newtable( state );

	unify::Matrix mat = unify::Matrix::MatrixPerspectiveFovLH( w, h, zn, zf );

	int index = 1;
	for( int c = 0; c < 4; ++c )
	{
		for( int r = 0; r < 4; ++r )
		{
			double value = mat( r, c );
			lua_pushnumber( state, value );
			lua_rawseti( state, -2, index++ );
		}
	}

	return 1;
}

static const luaL_Reg matrixFuncs[] =
{
	{ "makeidentity", Matrix_MakeIdentity },
	{ "makeperspectivefovlh", Matrix_MakePerspectiveFovLH },
	{ nullptr, nullptr }
};

int ExportMatrix( lua_State * state )
{
	luaL_newlib( state, matrixFuncs );
	lua_setglobal( state, "matrix" );
	return 1;
}