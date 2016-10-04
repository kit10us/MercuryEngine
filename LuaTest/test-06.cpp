#include <iostream>
#include <conio.h>
#include <cassert>

#include <lua.hpp>

#pragma comment( lib, "lua53" )

namespace test06
{

int V3New( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	float x = (float)lua_tonumber( state, 1 );
	float y = (float)lua_tonumber( state, 2 );
	float z = (float)lua_tonumber( state, 3 );

	lua_newtable( state ); // Create table.

	lua_pushstring( state, "x" );
	lua_pushnumber( state, x );
	lua_settable( state, -3 );

	lua_pushstring( state, "y" );
	lua_pushnumber( state, y );
	lua_settable( state, -3 );

	lua_pushstring( state, "z" );
	lua_pushnumber( state, z );
	lua_settable( state, -3 );

	return 1;
}

int V3ToString( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	luaL_checktype( state, 1, LUA_TTABLE );

	lua_getfield( state, 1, "x" );
	lua_getfield( state, 1, "y" );
	lua_getfield( state, 1, "z" );

	float x = (float)luaL_checknumber( state, -3 );
	float y = (float)luaL_checknumber( state, -2 );
	float z = (float)luaL_checknumber( state, -1 );


	std::string out = "{ " + x + ", " + y + ", " + z + " }";

	lua_pushstring( state, "working on it" );
	return 1;
}

static const luaL_Reg v3Funcs[] =
{
	{ "New", V3New },
	{ "ToString", V3ToString },
	{ nullptr, nullptr }
};

int ExportV3( lua_State * state )
{
	luaL_newlib( state, v3Funcs );
	lua_setglobal( state, "V3" );
	return 1;
}


int main( int argc, char ** argv )
{
	using namespace std;

	cout << "Vector test." << endl;

	lua_State * state = luaL_newstate();

	luaL_openlibs( state );

	/*
	// Single function...
	lua_pushcfunction( state, lua_NewV3 );
	lua_setglobal( state, "V3" );
	*/

	ExportV3( state );

	int result = luaL_dofile( state, "vector.lua" );
	if( result != LUA_OK )
	{
		cout << "Failed to load vector.lua!\n";
		if( lua_pcall( state, 0, 0, 0 ) != 0 )
		{
			cout << "LUA Error: \"" << lua_tostring( state, -1 ) << "\"\n";
		}
		return -1;
	}

	cout << "Press any key to continue..." << endl;

	while( !_getch() );
	lua_close( state );

	return 0;
}

}