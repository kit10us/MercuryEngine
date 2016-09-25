#include <iostream>
#include <conio.h>				  
#include <lua.hpp>

#pragma comment( lib, "lua53" )

namespace test02
{

void HelloLua()
{
	using namespace std;
	cout << "Hello, Lua!" << endl;
}

void print_error( lua_State * state )
{
	const char * message = lua_tostring( state, -1 );
	std::cout << "Lua Error: " << message << std::endl;
	lua_pop( state, 1 );
}

int main( int argc, char ** argv )
{
	using namespace std;

	cout << "Hello, World!" << endl;

	lua_State * state = luaL_newstate();

	luaL_openlibs( state );

	int result {};

	result = luaL_loadstring( state, "print(\"Hello, from inside Lua!\")" );
	if ( result != LUA_OK )
	{
		print_error( state );
		return 1;
	}

	result = lua_pcall( state, 0, LUA_MULTRET, 0 );
	if( result != LUA_OK )
	{
		print_error( state );
		return 1;
	}

	cout << "Press any key to continue..." << endl;

	while( !_getch() );
	lua_close( state );

	return 0;
}

}