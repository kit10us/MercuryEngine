// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ScriptEngine.h>
#include <dxilua/ExportScene.h>

#include "luaphysx/ExportPxSceneComponent.h"
#include "luaphysx/ExportPxController.h"
#include <dxiphysx/objectcomponent/CapsuleController.h>

#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/unify/ExportColor.h>
#include <dxilua/unify/ExportSize2.h>
#include <dxilua/unify/ExportSize3.h>
#include <dxilua/unify/ExportV2.h>
#include <dxilua/unify/ExportV3.h>

#include <PxPhysicsAPI.h>

#include <me/Game.h>

using namespace dxilua;
using namespace me;
using namespace scene;

static dxilua::ScriptEngine * g_luaSE;
static me::Game * g_game;

int PushPxSceneComponent( lua_State * state, dxiphysx::SceneComponent::ptr sceneComponent )
{
	PxSceneComponentProxy ** newProxy = (PxSceneComponentProxy**)(lua_newuserdata( state, sizeof( PxSceneComponentProxy* ) ));
	*newProxy = new PxSceneComponentProxy();
	(*newProxy)->sceneComponent = sceneComponent;
	(*newProxy)->scene = dynamic_cast< dxiphysx::SceneComponent * >( sceneComponent.get() );
	luaL_setmetatable( state, "PxSceneComponent" );
	return 1;
}

PxSceneComponentProxy * CheckPxSceneComponent( lua_State* state, int index )
{
	PxSceneComponentProxy* ud = *(PxSceneComponentProxy**)luaL_checkudata( state, index, "PxSceneComponent" );
	return ud;
}					 

int PxSceneComponent_AttachTo( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	PxSceneComponentProxy * pxScene = CheckPxSceneComponent( state, 1 );
	SceneProxy * scene = CheckScene( state, 2 );

	scene->scene->AddComponent( pxScene->sceneComponent );

	return 0;
}

int PxSceneComponent_CreateCapsuleController( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	PxSceneComponentProxy * pxScene = CheckPxSceneComponent( state, 1 );

	float radius = (float)lua_tonumber( state, 2 );
	float height = (float)lua_tonumber( state, 3 );											

	me::scene::IObjectComponent::ptr controller( new dxiphysx::objectcomponent::CapsuleController( g_game->GetOS(), pxScene->scene, radius, height ) );

	PushPxController( state, controller );

	return 1;
}

static const luaL_Reg PxSceneComponentFunctions[] =
{
	{ "AttachTo", PxSceneComponent_AttachTo },
	{ "CreateCapsuleController", PxSceneComponent_CreateCapsuleController },
	{ nullptr, nullptr }
};

int PxSceneComponent_Constructor( lua_State * state )
{
	dxiphysx::GameComponent * physics = dynamic_cast< dxiphysx::GameComponent *>(g_game->GetComponent( "PhysX", 0 ).get());
	ISceneComponent::ptr component( new dxiphysx::SceneComponent( g_game->GetOS(), physics ) );
	return PushPxSceneComponent( state, component );
}

int PxSceneComponent_Destructor( lua_State * state )
{
	PxSceneComponentProxy * proxy = CheckPxSceneComponent( state, 1 );
	delete proxy;
	return 0;
}

void RegisterPxSceneComponent( dxilua::ScriptEngine * luaSE, me::IGame * game )
{
	g_game = dynamic_cast< me::Game * >( game );
	g_luaSE = luaSE;

	luaSE->AddType( "PxSceneComponent", PxSceneComponentFunctions, 2, PxSceneComponent_Constructor, PxSceneComponent_Destructor );
}

