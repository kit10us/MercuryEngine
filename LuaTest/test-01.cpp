#include <iostream>
#include <conio.h>
#include <lua.hpp>

#pragma comment( lib, "lua53" )

namespace test01
{

void HelloLua()
{
	using namespace std;
	cout << "Hello, Lua!" << endl;
}

int main( int argc, char ** argv )
{
	using namespace std;

	cout << "Hello, World!" << endl;

	lua_State * state = luaL_newstate();
	lua_close( state );

	cout << "Press any key to continue..." << endl;

	while( !_getch() );

	return 0;
}

}