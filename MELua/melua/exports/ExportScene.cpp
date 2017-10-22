// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <melua/exports/ExportScene.h>
#include <melua/exports/ExportObject.h>
#include <melua/unify/ExportMatrix.h>
#include <melua/unify/ExportV3.h>
#include <melua/exports/ExportCameraComponent.h>
#include <melua/Util.h>
#include <me/Game.h>
#include <me/scene/SceneManager.h>
#include <me/scene/Scene.h>		

#include <unify/BSphere.h>

using namespace melua;
using namespace me;
using namespace scene;
using namespace object;

int PushScene( lua_State * state, me::scene::IScene * scene )
{
	SceneProxy ** sceneProxy = (SceneProxy**)(lua_newuserdata( state, sizeof( SceneProxy* ) ));
	*sceneProxy = new SceneProxy;
	luaL_setmetatable( state, "Scene" );
	(*sceneProxy)->scene = scene;
	return 1;
}

MELUADLL_API SceneProxy* CheckScene( lua_State* state, int index )
{
	SceneProxy* ud = *(SceneProxy**)luaL_checkudata( state, index, "Scene" );
	return ud;
}

int Scene_Name( lua_State * state )
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

	return PushUserType< ObjectProxy >( state, { object } );
} 

int Scene_NewCamera( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	SceneProxy * proxy = CheckScene( state, 1 );
	std::string name = lua_tostring( state, 2 );

	int t = lua_type( state, 3 );
	unify::Matrix mat = CheckUserType< MatrixProxy >( state, 3 )->matrix;

	ScriptEngine * se = ScriptEngine::GetInstance();
	auto game = se->GetGame();

	Object * child = proxy->scene->GetObjectAllocator()->NewObject( name );
	CameraComponent * cameraComponent = new CameraComponent();
	child->AddComponent( IObjectComponent::ptr( cameraComponent ) );

	cameraComponent->SetProjection( mat );

	return PushUserType< ObjectProxy >( state, { child }  );
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
		return 1;
	}
	else
	{
		return PushUserType< ObjectProxy >( state, { object } );
	}
}

int Scene_FindObjectsWithinRay( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	// Arguments...
	SceneProxy * sceneProxy = CheckScene( state, 1 );
	unify::V3< float > origin = CheckV3( state, 2 )->v3;
	unify::V3< float > direction = CheckV3( state, 3 )->v3;
	auto distance = (float)luaL_checknumber( state, 4 );
	auto ray = unify::Ray::RayFromOrginAndDirection( origin, direction );
			
	auto objects = sceneProxy->scene->FindObjectsWithinRay( ray, distance );

	lua_newtable( state );

	int i = 1;
	for( auto object : objects )
	{
		PushUserType< ObjectProxy >( state, { object.object } );
		lua_rawseti( state, -2, i );
		i++;
	}

	return 1;
}

int Scene_FindObjectsWithinSphere( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	// Arguments...
	SceneProxy * sceneProxy = CheckScene( state, 1 );
	unify::V3< float > origin = CheckV3( state, 2 )->v3;
	auto radius = (float)luaL_checknumber( state, 3 );
	auto sphere = unify::BSphere< float >::BSphere( origin, radius );

	auto objects = sceneProxy->scene->FindObjectsWithinSphere( sphere );

	// Create table for objects and distances...
	lua_newtable( state );

	int i = 1;
	for( auto object : objects )
	{
		// Create table (within table) for object and distance pair...
		lua_newtable( state );
		int t = lua_gettop( state );

		// Push object table entry...
		lua_pushstring( state, "object" );
		PushUserType< ObjectProxy >( state, { object.object } );
		lua_rawset( state, -3 );

		// Push distance table enttry...
		lua_pushstring( state, "distance" );
		lua_pushnumber( state, object.distance );
		lua_rawset( state, -3 );

		// finish sub-table (of object and distance pair).
		lua_rawseti( state, -2, i );

		i++;
	}

	return 1;
}

int Scene_GetObjectCount( lua_State * state )
{
	int args = lua_gettop( state );

	SceneProxy * sceneProxy = CheckScene( state, 1 );
	return Push( state, sceneProxy->scene->GetObjectAllocator()->Count() );
}

int Scene_SendCommand( lua_State * state )
{
	int args = lua_gettop( state );

	SceneProxy * sceneProxy = CheckScene( state, 1 );

	std::string command = lua_tostring( state, 2 );

	std::string type = GetTypename( state, 3 );

	std::string extra = "";
	
	if( args > 2 )
	{
		extra = lua_tostring( state, 3 );
	}
	auto result = sceneProxy->scene->SendCommand( command, extra );
	lua_pushstring( state, result.c_str() );
	return 1;
}

int Scene_Constructor(lua_State * state)
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	auto sceneManager = se->GetGame()->GetComponentT< me::scene::SceneManager >();
	return PushScene(state, sceneManager->GetCurrentScene());
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
		{ "Name", Scene_Name },
		{ "NewObject", Scene_NewObject },
		{ "NewCamera", Scene_NewCamera },
		{ "FindObject", Scene_FindObject },
		{ "FindObjectsWithinRay", Scene_FindObjectsWithinRay },
		{ "FindObjectsWithinSphere", Scene_FindObjectsWithinSphere },
		{ "GetObjectCount", Scene_GetObjectCount },
		{ "SendCommand", Scene_SendCommand },
		{ nullptr, nullptr }
	};

	ScriptEngine * se = ScriptEngine::GetInstance();
	se->AddType( { "Scene", memberFunctions, sizeof( memberFunctions ) / sizeof( luaL_Reg ), Scene_Constructor, Scene_Destructor } );
}