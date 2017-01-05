// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ScriptEngine.h>
#include <dxilua/ExportScene.h>
#include <dxilua/ExportObject.h>
#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/ExportCameraComponent.h>
#include <me/Game.h>
#include <me/scene/SceneManager.h>

using namespace dxilua;
using namespace me;
using namespace scene;

DXILUADLL_API SceneProxy* CheckScene( lua_State* state, int index )
{
	SceneProxy* ud = *(SceneProxy**)luaL_checkudata( state, index, "Scene" );
	return ud;
}

int Scene_FindObject( lua_State * state )
{
	int args = lua_gettop( state );

	SceneProxy * sceneProxy = CheckScene( state, 1 );

	std::string name = lua_tostring( state, 2 );

	Object * object = sceneProxy->scene->FindObject( name );

	ObjectProxy ** objectProxy = (ObjectProxy**)(lua_newuserdata( state, sizeof( ObjectProxy* ) ));
	*objectProxy = new ObjectProxy;
	luaL_setmetatable( state, "Object" );
	(*objectProxy)->object = object;

	return 1;
}

int Scene_SetSize( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	SceneProxy * sceneProxy = CheckScene( state, 1 );

	float width = (float)lua_tonumber( state, 2 );
	float height = (float)lua_tonumber( state, 3 );

	auto game = ScriptEngine::GetGame();

	sceneProxy->scene->SetSize( unify::Size< float >( width, height ) );

	return 0;
}
			   
int Scene_NewObject( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	SceneProxy * proxy = CheckScene( state, 1 );
	std::string name = lua_tostring( state, 2 );

	auto object = proxy->scene->NewObject( name );

	ObjectProxy ** childProxy = (ObjectProxy**)(lua_newuserdata( state, sizeof( ObjectProxy* ) ));
	*childProxy = new ObjectProxy;
	luaL_setmetatable( state, "Object" );
	(*childProxy)->object = object;

	return 1;
}

int Scene_NewCamera( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	SceneProxy * proxy = CheckScene( state, 1 );
	std::string name = lua_tostring( state, 2 );

	int t = lua_type( state, 3 );
	unify::Matrix mat = CheckMatrix( state, 3 );

	auto game = ScriptEngine::GetGame();

	Object * child = proxy->scene->NewObject( name );
	CameraComponent * cameraComponent = new CameraComponent( game->GetOS() );
	child->AddComponent( IObjectComponent::ptr( cameraComponent ) );

	cameraComponent->SetProjection( mat );

	ObjectProxy ** childProxy = (ObjectProxy**)(lua_newuserdata( state, sizeof( ObjectProxy* ) ));
	*childProxy = new ObjectProxy;
	luaL_setmetatable( state, "Object" );
	(*childProxy)->object = child;

	return 1;
}

static const luaL_Reg SceneFunctions[] =
{
	{ "FindObject", Scene_FindObject },
	{ "SetSize", Scene_SetSize },
	{ "NewObject", Scene_NewObject },
	{ "NewCamera", Scene_NewCamera },
	{ nullptr, nullptr }
};

int Scene_Constructor( lua_State * state )
{
	std::string name = luaL_checkstring( state, -1 );
	SceneProxy ** sceneProxy = (SceneProxy**)(lua_newuserdata( state, sizeof( SceneProxy* ) ));
	*sceneProxy = new SceneProxy;
	luaL_setmetatable( state, "Scene" );

	auto game = dynamic_cast< Game * >( ScriptEngine::GetGame() );

	auto x = game->GetComponent( "SceneManager", 0 );
	if( !x ) game->ReportError( me::ErrorLevel::Failure, "Lua", "Could not find scene manager!" );

	scene::SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(x.get());

	(*sceneProxy)->scene = sceneManager->Add( name );
	return 1;
}

int Scene_Destructor( lua_State * state )
{			
	SceneProxy * sceneProxy = CheckScene( state, 1 );
	delete sceneProxy;
	return 0;
}
			          
void RegisterScene( lua_State * state )
{
	lua_register( state, "Scene", Scene_Constructor );
	luaL_newmetatable( state, "Scene" );
	lua_pushcfunction( state, Scene_Destructor ); lua_setfield( state, -2, "__gc" );
	lua_pushvalue( state, -1 ); lua_setfield( state, -2, "__index" );
	luaL_setfuncs( state, SceneFunctions, 0 );
	lua_pop( state, 1 );
}