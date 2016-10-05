#include <iostream>
#include <conio.h>
#include <cassert>

#include <lua.hpp>

#pragma comment( lib, "lua53" )

namespace test07
{

struct Object
{
	Object( std::string nameIn )
		: name( nameIn )
	{
	}

	std::string name;
	std::string scene;
	std::string parent;
};

int Object_New( lua_State * state )
{
	int argc = lua_gettop( state );
	std::string name = luaL_checkstring( state, 1 );

	Object ** o = (Object**)lua_newuserdata( state, sizeof( Object* ) );  
	*o = new Object( name );

	luaL_getmetatable( state, "Object" );
	
	lua_getglobal( state, "Object" );

	lua_setmetatable( state, -2 );

	return 1;
}

int Object_Delete( lua_State * state )
{
	return 0;
}

int Object_Name( lua_State * state )
{
	int args = lua_gettop( state );
	
	Object * o = *(Object**)luaL_checkudata( state, 1, "Object" );

	lua_pushstring( state, "testdata" );

	return 1;
}								  

void registerObject( lua_State * state )
{
	luaL_Reg ObjectFunctions[] =
	{
		{ "new", Object_New },
		{ "Name", Object_Name },
		{ "__gc", Object_Delete },
		{ 0, 0 }
	};

	luaL_newmetatable( state, "Object" );

	luaL_newlib( state, ObjectFunctions );
	lua_pushvalue( state, -1 );
	lua_setfield( state, -1, "__index" );
	lua_setglobal( state, "Object" );
}

int main( int argc, char ** argv )
{
	using namespace std;

	cout << "Class test." << endl;

	lua_State * state = luaL_newstate();

	luaL_openlibs( state );

	registerObject( state );

	int result = luaL_dofile( state, "class.lua" );
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