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

Object* CheckObject( lua_State* state, int index )
{
	Object* ud = *(Object**)luaL_checkudata( state, index, "Object" );
	return ud;
}						  

int Object_Name( lua_State * state )
{										  	
	int argc = lua_gettop( state );

	int x = lua_type( state, 1 );
	
	Object * o = CheckObject( state, 1 );

	lua_pushstring( state, o->name.c_str() );

	return 1;
}		

static const luaL_Reg ObjectFunctions[] =
{
	{ "Name", Object_Name },
	{ 0, 0 }
};

						 
int Object_New( lua_State * state )
{
	std::string name = luaL_checkstring( state, -1 );
	Object ** object = (Object**)(lua_newuserdata( state, sizeof( Object* ) ));
	*object = new Object( name );

	luaL_setmetatable( state, "Object" );
	return 1;
}

int Object_Delete( lua_State * state )
{
	Object * o = CheckObject( state, 1 );
	delete o;
	return 0;
}

void registerObject( lua_State * state )
{
	lua_register( state, "Object", Object_New );
	luaL_newmetatable( state, "Object" );
	lua_pushcfunction( state, Object_Delete ); lua_setfield( state, -2, "__gc" );
	lua_pushvalue( state, -1 ); lua_setfield( state, -2, "__index" );
	luaL_setfuncs( state, ObjectFunctions, 0 );
	lua_pop( state, 1 );
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