// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ExportDebug.h>
#include <dxilua/DXILua.h>

using namespace dxilua;
using namespace dxi;

extern "C"
int Debug_Write( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	std::string log = lua_tostring( state, 1 );

	OutputDebugStringA( log.c_str() );

	return 0;
}

extern "C"
int Debug_WriteLine( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	int type = lua_type( state, 1 );

	std::string log = lua_tostring( state, 1 );


	if ( log.empty() )
	{
		OutputDebugStringA( "\n" );
	}
	else
	{
		OutputDebugStringA( (log + "\n").c_str() );
	}

	return 0;
}

static const luaL_Reg debugFuncs[] =
{
	{ "Write", Debug_Write },
	{ "WriteLine", Debug_WriteLine },
	{ nullptr, nullptr }
};

int ExportDebug( lua_State * state )
{
	luaL_newlib( state, debugFuncs );
	lua_setglobal( state, "Debug" );
	return 1;
}

