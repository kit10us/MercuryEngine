#include <iostream>
#include <conio.h>

#include <lua.hpp>

#pragma comment( lib, "lua53" )

namespace test05
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

	cout << "Attempting to check for two existing functions, OnStart, OnUpdate, and a non-existing function Missing." << endl;

	lua_State * state = luaL_newstate();

	luaL_openlibs( state );

	luaL_dofile( state, "module.lua" );

	// Find function...
	{
		int i = lua_getglobal( state, "OnStart" );

		if ( i != 0 )
		{
			if ( lua_pcall( state, 0, 0, 0 ) != 0 )
			{
				printf( "Failed to call function! \"%s\"", lua_tostring( state, -1 ) );
			}
		}
	}
	
	{
		int i = lua_getglobal( state, "OnUpdate" );
		if( i != 0 )
		{
			if( lua_pcall( state, 0, 0, 0 ) != 0 )
			{
				printf( "Failed to call function! \"%s\"", lua_tostring( state, -1 ) );
			}
		}
	}

	{
		int i = lua_getglobal( state, "Missing" );
		if( i != 0 )
		{
			if( lua_pcall( state, 0, 0, 0 ) != 0 )
			{
				printf( "Failed to call function! \"%s\"", lua_tostring( state, -1 ) );
			}
		}
	}

	cout << "Press any key to continue..." << endl;

	while( !_getch() );
	lua_close( state );

	return 0;
}

}