// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/Matrix.h>
#include <dxilua/V3.h>

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

int PushMatrix( lua_State* state, const unify::Matrix & mat )
{
	lua_newtable( state );

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
