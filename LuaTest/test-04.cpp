#include <iostream>
#include <conio.h>

#include <lua.hpp>

#pragma comment( lib, "lua53" )

namespace test04
{

bool script_Start( lua_State * state )
{
	lua_getglobal( state, "Start" );
	lua_call( state, 0, 0 );
	return true;
}

bool script_Update( lua_State * state )
{
	lua_getglobal( state, "Update" );
	lua_call( state, 0, 0 );
	return true;
}

bool script_Missing( lua_State * state )
{
	lua_getglobal( state, "Missing" );
	lua_call( state, 0, 0 );
	return true;
}


int main( int argc, char ** argv )
{
	using namespace std;

	cout << "Attempting to check for two existing functions, Start, Update, and a non-existing function Missing." << endl;

	lua_State * state = luaL_newstate();

	luaL_openlibs( state );

	luaL_dofile( state, "reflect.lua" );

	/*

	if ( script_Start( state ) == true )
	{
		cout << "Start found - PASS!";
	}
	else
	{
		cout << "Start found - FAIL!";
	}

	if( script_Update( state ) == true )
	{
		cout << "Update found - PASS!";
	}
	else
	{
		cout << "Update found - FAIL!";
	}

	if( script_Missing( state ) == true )
	{
		cout << "Missing found - FAIL!";
	}
	else
	{
		cout << "Missing found - PASS!";
	} */

	//#define lua_getglobal(L,s)  lua_getfield(L, LUA_GLOBALSINDEX, s)

	/*
	// Find function...
	lua_Debug ar;
	lua_getglobal( state, "Start" );
	lua_getinfo( state, ">S", &ar );
	printf( "%d\n", ar.linedefined );
	*/

	// List all globals...
	lua_pushglobaltable( state );       // Get global table
	lua_pushnil( state );               // put a nil key on stack
	while( lua_next( state, -2 ) != 0 ) { // key(-1) is replaced by the next key(-1) in table(-2)
		cout << lua_tostring( state, -2 ) << endl;  // Get key(-2) name
		lua_pop( state, 1 );               // remove value(-1), now key on top at(-1)
	}
	lua_pop( state, 1 );



	cout << "Press any key to continue..." << endl;

	while( !_getch() );
	lua_close( state );

	return 0;
}

}