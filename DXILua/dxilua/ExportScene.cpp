// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ScriptEngine.h>
#include <dxilua/ExportScene.h>
#include <dxilua/ExportObject.h>
#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/ExportCameraComponent.h>
#include <me/Game.h>
#include <me/scene/SceneManager.h>

#include <luaw/LuaClass.h>

using namespace dxilua;
using namespace me;
using namespace scene;
using namespace object;

DXILUADLL_API SceneProxy* CheckScene( lua_State* state, int index )
{
	SceneProxy* ud = *(SceneProxy**)luaL_checkudata( state, index, "Scene" );
	return ud;
}

int Scene_FindScene( lua_State * state )
{
	auto game = ScriptEngine::GetGame();
	int args = lua_gettop( state );

	std::string name = lua_tostring( state, 1 );

	auto x = game->GetComponent( "SceneManager" );
	if( !x ) game->ReportError( me::ErrorLevel::Failure, "Lua", "Could not find scene manager!" );

	scene::SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(x.get());

	SceneProxy ** sceneProxy = (SceneProxy**)(lua_newuserdata( state, sizeof( SceneProxy* ) ));
	*sceneProxy = new SceneProxy;
	luaL_setmetatable( state, "Scene" );
	(*sceneProxy)->scene = sceneManager->FindScene( name );
	return 1;
}

int Scene_GetName( lua_State * state )
{
	int args = lua_gettop( state );

	SceneProxy * sceneProxy = CheckScene( state, 1 );
	lua_pushstring( state, sceneProxy->scene->GetName().c_str() );
	return 1;
}

int Scene_NewObject( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	SceneProxy * proxy = CheckScene( state, 1 );
	std::string name = lua_tostring( state, 2 );

	auto object = proxy->scene->GetObjectAllocator()->NewObject( name );

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

	Object * child = proxy->scene->GetObjectAllocator()->NewObject( name );
	CameraComponent * cameraComponent = new CameraComponent();
	child->AddComponent( IObjectComponent::ptr( cameraComponent ) );

	cameraComponent->SetProjection( mat );

	ObjectProxy ** childProxy = (ObjectProxy**)(lua_newuserdata( state, sizeof( ObjectProxy* ) ));
	*childProxy = new ObjectProxy;
	luaL_setmetatable( state, "Object" );
	(*childProxy)->object = child;

	return 1;
}

int Scene_FindObject( lua_State * state )
{
	int args = lua_gettop( state );

	SceneProxy * sceneProxy = CheckScene( state, 1 );

	std::string name = lua_tostring( state, 2 );
	
	Object * object = sceneProxy->scene->FindObject( name );

	if ( !object )
	{
		lua_pushnil( state );
	}
	else
	{
		ObjectProxy ** objectProxy = (ObjectProxy**)(lua_newuserdata( state, sizeof( ObjectProxy* ) ));
		*objectProxy = new ObjectProxy;
		luaL_setmetatable( state, "Object" );
		(*objectProxy)->object = object;
	}

	return 1;
}

int Scene_GetObjectCount( lua_State * state )
{
	int args = lua_gettop( state );

	SceneProxy * sceneProxy = CheckScene( state, 1 );
	lua_pushnumber( state, sceneProxy->scene->GetObjectAllocator()->Count() );
	return 1;
}

int Scene_Constructor( lua_State * state )
{
	auto game = dynamic_cast< Game * >( ScriptEngine::GetGame() );

	std::string name = luaL_checkstring( state, -1 );

	auto x = game->GetComponent( "SceneManager" );
	if( !x ) game->ReportError( me::ErrorLevel::Failure, "Lua", "Could not find scene manager!" );

	scene::SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(x.get());

	SceneProxy ** sceneProxy = (SceneProxy**)(lua_newuserdata( state, sizeof( SceneProxy* ) ));
	*sceneProxy = new SceneProxy;
	luaL_setmetatable( state, "Scene" );
	(*sceneProxy)->scene = sceneManager->AddScene( name );
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
	const luaL_Reg memberFunctions[] =
	{
		{ "GetName", Scene_GetName },
		{ "NewObject", Scene_NewObject },
		{ "NewCamera", Scene_NewCamera },
		{ "FindObject", Scene_FindObject },
		{ "GetObjectCount", Scene_GetObjectCount },
		{ nullptr, nullptr }
	};

	ScriptEngine * se = ScriptEngine::GetInstance();
	se->AddType( "Scene", memberFunctions, sizeof( memberFunctions ) / sizeof( luaL_Reg ), Scene_Constructor, Scene_Destructor );

	const luaL_Reg SceneStaticFunctions[] =
	{
		{ "FindScene", Scene_FindScene },
		{ nullptr, nullptr }
	};
	
	lua_register( state, "FindScene", Scene_FindScene );
}