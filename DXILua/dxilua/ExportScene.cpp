// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ExportScene.h>
#include <dxilua/DXILua.h>

using namespace dxilua;
using namespace dxi;

extern "C"
int Scene_AddObject( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	std::string sceneName = lua_tostring( state, 1 );
	std::string name = lua_tostring( state, 2 );

	auto game = ScriptEngine::GetGame();

	scene::Scene * scene = game->GetSceneManager()->Find( sceneName ).get();
	scene->Add( name );

	return 0;
}

extern "C"
int Scene_SetCamera( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	std::string sceneName = lua_tostring( state, 1 );
	std::string camera = lua_tostring( state, 2 );

	auto game = ScriptEngine::GetGame();

	scene::Scene * scene = game->GetSceneManager()->Find( sceneName ).get();
	scene->SetCamera( camera );

	return 0;
}

extern "C"
int Scene_SetSize( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	std::string sceneName = lua_tostring( state, 1 );
	float width = (float)lua_tonumber( state, 2 );
	float height = (float)lua_tonumber( state, 3 );

	auto game = ScriptEngine::GetGame();

	scene::Scene * scene = game->GetSceneManager()->Find( sceneName ).get();
	scene->SetSize( unify::Size< float >( width, height ) );

	return 0;
}

static const luaL_Reg sceneFuncs[] =
{
	{ "addobject", Scene_AddObject },
	{ "setcamera", Scene_SetCamera },
	{ "setsize", Scene_SetSize },
	{ nullptr, nullptr }
};

int ExportScene( lua_State * state )
{
	luaL_newlib( state, sceneFuncs );
	lua_setglobal( state, "scene" );
	return 1;
}

