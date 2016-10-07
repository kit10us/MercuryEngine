// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ExportScenes.h>
#include <dxilua/DXILua.h>

using namespace dxilua;
using namespace dxi;

extern "C"
int Scenes_LoadScene( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	std::string name = lua_tostring( state, 1 );
	std::string path = lua_tostring( state, 2 );

	auto game = ScriptEngine::GetGame();

	game->GetSceneManager()->Load( name, path );

	return 0;
}

extern "C"
int Scenes_AddScene( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	std::string name = lua_tostring( state, 1 );

	auto game = ScriptEngine::GetGame();

	game->GetSceneManager()->Add( name );

	return 0;
}

static const luaL_Reg scenesFuncs[] =
{
	{ "load", Scenes_LoadScene },
	{ "add", Scenes_AddScene },
	{ nullptr, nullptr }
};

int ExportScenes( lua_State * state )
{
	luaL_newlib( state, scenesFuncs );
	lua_setglobal( state, "scenes" );
	return 1;
}
