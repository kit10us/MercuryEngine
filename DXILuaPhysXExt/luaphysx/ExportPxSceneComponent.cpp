// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/DXILua.h>
#include "luaphysx/ExportPxSceneComponent.h"

#include <dxilua/Matrix.h>
#include <dxilua/Color.h>
#include <dxilua/Size2.h>
#include <dxilua/Size3.h>
#include <dxilua/V2.h>
#include <dxilua/V3.h>

#include <PxPhysicsAPI.h>

using namespace dxilua;
using namespace dxi;

static dxilua::ScriptEngine * g_luaSE;

int PushPxSceneComponent( lua_State * state, dxiphysx::SceneComponent::ptr sceneComponent )
{
	PxSceneComponentProxy ** newProxy = (PxSceneComponentProxy**)(lua_newuserdata( state, sizeof( PxSceneComponentProxy* ) ));
	*newProxy = new PxSceneComponentProxy();
	(*newProxy)->sceneComponent = sceneComponent;
	luaL_setmetatable( state, "PxSceneComponent" );
	return 1;
}

PxSceneComponentProxy * CheckPxSceneComponent( lua_State* state, int index )
{
	PxSceneComponentProxy* ud = *(PxSceneComponentProxy**)luaL_checkudata( state, index, "PxSceneComponent" );
	return ud;
}					 


int PxSceneComponent_CreateBox( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	auto v3 = CheckV3( state, 1 );
	/*

	std::shared_ptr< physx::PxShape > shape( 
	PxShape * cube = m_physics->createShape( PxBoxGeometry( v3.x, v3.y, v3.z ), *m_material );

	  */

	int x( 0 ); x;

	return 0;
}

static const luaL_Reg PxSceneComponentFunctions[] =
{
	{ "Create", PxSceneComponent_CreateBox },
	{ nullptr, nullptr }
};

void RegisterPxSceneComponent( dxilua::ScriptEngine * luaSE )
{
	g_luaSE = luaSE;

	luaSE->AddLibrary( "PxShapeComponent", PxSceneComponentFunctions, 1 );
}

