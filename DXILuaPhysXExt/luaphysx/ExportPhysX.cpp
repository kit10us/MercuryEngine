// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ScriptEngine.h>
#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/unify/ExportColor.h>
#include <dxilua/unify/ExportSize2.h>
#include <dxilua/unify/ExportSize3.h>
#include <dxilua/unify/ExportV2.h>
#include <dxilua/unify/ExportV3.h>
#include <dxilua/ExportTerra.h>

#include <mephysx/GameComponent.h>
#include <mephysx/SceneComponent.h>
#include <mephysx/objectcomponent/BoxCollider.h>
#include <mephysx/objectcomponent/SphereCollider.h>
#include <mephysx/objectcomponent/CapsuleCollider.h>
#include <mephysx/objectcomponent/RigidBody.h>
#include <mephysx/objectcomponent/HeightFieldCollider.h>

#include <luaphysx/ExportPhysX.h>
#include <luaphysx/ExportPxSceneComponent.h>
#include <luaphysx/ExportPxShape.h>
#include <luaphysx/ExportPxRigidBody.h>
#include <luaphysx/ExportPxRigidStatic.h>

#include <PxPhysicsAPI.h>

#include <me/Game.h>

using namespace dxilua;
using namespace me;


std::shared_ptr< physx::PxPhysics > GetPhysX()
{
	return std::shared_ptr< physx::PxPhysics >();
}

static dxilua::ScriptEngine * g_luaSE;
static me::Game * g_game;

int PhysX_CreateBoxCollider( lua_State * state )
{
	int argc = lua_gettop( state );
	assert( argc == 1 );

	unify::V3< float > halfExt( CheckV3( state, 1 ) );

	mephysx::GameComponent * physics = dynamic_cast< mephysx::GameComponent *>(g_game->GetComponent( "PhysX", 0 ).get());
	me::scene::IObjectComponent::ptr collider( new mephysx::objectcomponent::BoxCollider( physics, halfExt ) );

	PushPxShape( state, collider );

	return 1;
}

int PhysX_CreateSphereCollider( lua_State * state )
{
	int argc = lua_gettop( state );
	assert( argc == 1 );

	float halfExt( (float)luaL_checknumber( state, 1 ) );

	mephysx::GameComponent * physics = dynamic_cast< mephysx::GameComponent *>(g_game->GetComponent( "PhysX", 0 ).get());
	me::scene::IObjectComponent::ptr collider( new mephysx::objectcomponent::SphereCollider( physics, halfExt ) );

	PushPxShape( state, collider );

	return 1;
}

int PhysX_CreateCapsuleCollider( lua_State * state )
{
	int argc = lua_gettop( state );
	assert( argc == 2 );

	mephysx::GameComponent * physics = dynamic_cast< mephysx::GameComponent *>(g_game->GetComponent( "PhysX", 0 ).get());
	
	float radius( (float)luaL_checknumber( state, 1 ) );
	float halfHeight( (float)luaL_checknumber( state, 2 ) );

	me::scene::IObjectComponent::ptr collider( new mephysx::objectcomponent::CapsuleCollider( physics, radius, halfHeight ) );

	PushPxShape( state, collider );

	return 1;
}

int PhysX_CreateHeightFieldCollider( lua_State * state )
{
	int argc = lua_gettop( state );
	assert( argc == 3 );

	mephysx::GameComponent * physics = dynamic_cast< mephysx::GameComponent *>(g_game->GetComponent( "PhysX", 0 ).get());
	
	TerraProxy * terra = CheckTerra( state, 1 );

	unify::RowColumn< unsigned int > rcCount( (unsigned int)luaL_checknumber( state, 3 ), (unsigned int)luaL_checknumber( state, 2 ) );

	me::scene::IObjectComponent::ptr collider( new mephysx::objectcomponent::HeightFieldCollider( physics, terra->terra, rcCount ) );

	PushPxShape( state, collider );

	return 1;
}

int PhysX_CreateRigidBody( lua_State * state )
{
	int argc = lua_gettop( state );
	assert( argc == 0 );

	mephysx::GameComponent * physics = dynamic_cast< mephysx::GameComponent *>(g_game->GetComponent( "PhysX", 0 ).get());
	me::scene::IObjectComponent::ptr body( new mephysx::objectcomponent::RigidBody( physics ) );

	PushPxRigidBody( state, body );

	return 1;
}

int PhysX_CreateRigidStatic( lua_State * state )
{
	int argc = lua_gettop( state );
	assert( argc == 0 );

	mephysx::GameComponent * physics = dynamic_cast< mephysx::GameComponent *>(g_game->GetComponent( "PhysX", 0 ).get());
	me::scene::IObjectComponent::ptr body( new mephysx::objectcomponent::RigidStatic( physics ) );

	PushPxRigidStatic( state, body );

	return 1;
}

static const luaL_Reg PhysXFunctions[] =
{
	{ "CreateBoxCollider", PhysX_CreateBoxCollider },
	{ "CreateSphereCollider", PhysX_CreateSphereCollider },
	{ "CreateCapsuleCollider", PhysX_CreateCapsuleCollider },
	{ "CreateHeightFieldCollider", PhysX_CreateHeightFieldCollider },
	{ "CreateRigidBody", PhysX_CreateRigidBody },
	{ "CreateRigidStatic", PhysX_CreateRigidStatic }
};

void RegisterPhysX( dxilua::ScriptEngine * luaSE, me::IGame * game )
{
	g_luaSE = luaSE;
	g_game = dynamic_cast< Game * >(game);

	luaSE->AddLibrary( "PhysX", PhysXFunctions, sizeof( PhysXFunctions ) / sizeof( luaL_Reg ) );
}

