// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/DXILua.h>
#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/unify/ExportColor.h>
#include <dxilua/unify/ExportSize2.h>
#include <dxilua/unify/ExportSize3.h>
#include <dxilua/unify/ExportV2.h>
#include <dxilua/unify/ExportV3.h>
#include <dxilua/ExportTerra.h>

#include <dxiphysx/GameComponent.h>
#include <dxiphysx/SceneComponent.h>
#include <dxiphysx/objectcomponent/BoxCollider.h>
#include <dxiphysx/objectcomponent/SphereCollider.h>
#include <dxiphysx/objectcomponent/RigidBody.h>
#include <dxiphysx/objectcomponent/HeightFieldCollider.h>

#include <luaphysx/ExportPhysX.h>
#include <luaphysx/ExportPxSceneComponent.h>
#include <luaphysx/ExportPxShape.h>
#include <luaphysx/ExportPxRigidBody.h>
#include <luaphysx/ExportPxRigidStatic.h>

#include <PxPhysicsAPI.h>

using namespace dxilua;
using namespace dxi;


std::shared_ptr< physx::PxPhysics > GetPhysX()
{
	return std::shared_ptr< physx::PxPhysics >();
}

static dxilua::ScriptEngine * g_luaSE;
static dxi::core::Game * g_game;

int PhysX_CreateBoxCollider( lua_State * state )
{
	int argc = lua_gettop( state );
	assert( argc == 1 );

	unify::V3< float > halfExt( CheckV3( state, 1 ) );

	dxiphysx::GameComponent * physics = dynamic_cast< dxiphysx::GameComponent *>(g_game->GetComponent( "PhysX", 0 ).get());
	dxiphysx::objectcomponent::BoxCollider::ptr collider( new dxiphysx::objectcomponent::BoxCollider( g_game->GetOS(), physics, halfExt ) );

	PushPxShape( state, collider );

	return 1;
}

int PhysX_CreateSphereCollider( lua_State * state )
{
	int argc = lua_gettop( state );
	assert( argc == 1 );

	float halfExt( (float)luaL_checknumber( state, 1 ) );

	dxiphysx::GameComponent * physics = dynamic_cast< dxiphysx::GameComponent *>(g_game->GetComponent( "PhysX", 0 ).get());
	dxiphysx::objectcomponent::BoxCollider::ptr collider( new dxiphysx::objectcomponent::SphereCollider( g_game->GetOS(), physics, halfExt ) );

	PushPxShape( state, collider );

	return 1;
}

int PhysX_CreateHeightFieldCollider( lua_State * state )
{
	int argc = lua_gettop( state );
	assert( argc == 3 );

	dxiphysx::GameComponent * physics = dynamic_cast< dxiphysx::GameComponent *>(g_game->GetComponent( "PhysX", 0 ).get());
	
	TerraProxy * terra = CheckTerra( state, 1 );

	unify::RowColumn< unsigned int > rcCount( (unsigned int)luaL_checknumber( state, 3 ), (unsigned int)luaL_checknumber( state, 2 ) );

	dxiphysx::objectcomponent::HeightFieldCollider::ptr collider( new dxiphysx::objectcomponent::HeightFieldCollider( g_game->GetOS(), physics, terra->terra, rcCount ) );

	PushPxShape( state, collider );

	return 1;
}

int PhysX_CreateRigidBody( lua_State * state )
{
	int argc = lua_gettop( state );
	assert( argc == 0 );

	dxiphysx::GameComponent * physics = dynamic_cast< dxiphysx::GameComponent *>(g_game->GetComponent( "PhysX", 0 ).get());
	dxiphysx::objectcomponent::RigidBody::ptr body( new dxiphysx::objectcomponent::RigidBody( g_game->GetOS(), physics ) );

	PushPxRigidBody( state, body );

	return 1;
}

int PhysX_CreateRigidStatic( lua_State * state )
{
	int argc = lua_gettop( state );
	assert( argc == 0 );

	dxiphysx::GameComponent * physics = dynamic_cast< dxiphysx::GameComponent *>(g_game->GetComponent( "PhysX", 0 ).get());
	dxiphysx::objectcomponent::RigidStatic::ptr body( new dxiphysx::objectcomponent::RigidStatic( g_game->GetOS(), physics ) );

	PushPxRigidStatic( state, body );

	return 1;
}

static const luaL_Reg PhysXFunctions[] =
{
	{ "CreateBoxCollider", PhysX_CreateBoxCollider },
	{ "CreateSphereCollider", PhysX_CreateSphereCollider },
	{ "CreateHeightFieldCollider", PhysX_CreateHeightFieldCollider },
	{ "CreateRigidBody", PhysX_CreateRigidBody },
	{ "CreateRigidStatic", PhysX_CreateRigidStatic }
};

void RegisterPhysX( dxilua::ScriptEngine * luaSE, dxi::core::Game * game )
{
	g_luaSE = luaSE;
	g_game = game;

	luaSE->AddLibrary( "PhysX", PhysXFunctions, sizeof( PhysXFunctions ) / sizeof( luaL_Reg ) );
}

