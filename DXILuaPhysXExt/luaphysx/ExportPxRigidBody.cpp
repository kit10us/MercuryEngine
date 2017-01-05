// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ScriptEngine.h>
#include <luaphysx/ExportPxRigidBody.h>

#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/unify/ExportColor.h>
#include <dxilua/unify/ExportSize2.h>
#include <dxilua/unify/ExportSize3.h>
#include <dxilua/unify/ExportV2.h>
#include <dxilua/unify/ExportV3.h>
#include <dxilua/ExportObject.h>

#include <PxPhysicsAPI.h>

#include <me/Game.h>

using namespace dxilua;

static dxilua::ScriptEngine * g_luaSE;
static me::Game * g_game;

int PushPxRigidBody( lua_State * state, dxiphysx::objectcomponent::RigidBody::ptr component )
{
	PxRigidBodyProxy ** newProxy = (PxRigidBodyProxy**)(lua_newuserdata( state, sizeof( PxRigidBodyProxy* ) ));
	*newProxy = new PxRigidBodyProxy();
	(*newProxy)->component = component;
	(*newProxy)->body = dynamic_cast< dxiphysx::objectcomponent::RigidBody * >( component.get() )->GetRigidBody();
	luaL_setmetatable( state, "PxRigidBody" );
	return 1;
}

PxRigidBodyProxy* CheckPxRigidBody( lua_State* state, int index )
{
	PxRigidBodyProxy * ud = *(PxRigidBodyProxy**)luaL_checkudata( state, index, "PxRigidBody" );
	return ud;
}					 
							  
int PxRigidBody_AttachTo( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	PxRigidBodyProxy * pxBody = CheckPxRigidBody( state, 1 );
	ObjectProxy * object = CheckObject( state, 2 );

	object->object->AddComponent( pxBody->component );

	return 0;
}

int PxRigidBody_SetDisableGravity( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	PxRigidBodyProxy * pxBody = CheckPxRigidBody( state, 1 );
	bool disableGravity = lua_toboolean( state, 2 ) ? true : false;

	pxBody->body->setActorFlag( physx::PxActorFlag::eDISABLE_GRAVITY, disableGravity );

	return 0;
}

int PxRigidBody_GetDisableGravity( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	PxRigidBodyProxy * pxBody = CheckPxRigidBody( state, 1 );
	bool disableGravity = ((pxBody->body->getActorFlags() & physx::PxActorFlag::eDISABLE_GRAVITY) == physx::PxActorFlag::eDISABLE_GRAVITY) ? true : false;

	lua_pushboolean( state, disableGravity ? 1 : 0 );
	return 1;
}

int PxRigidBody_SetKinematic( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	PxRigidBodyProxy * pxBody = CheckPxRigidBody( state, 1 );
	bool enable = lua_toboolean( state, 2 ) ? true : false;

	pxBody->body->setRigidBodyFlag( physx::PxRigidBodyFlag::eKINEMATIC, enable );

	return 0;
}

int PxRigidBody_GetKinematic( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	PxRigidBodyProxy * pxBody = CheckPxRigidBody( state, 1 );
	bool enabled = ((pxBody->body->getRigidBodyFlags() & physx::PxRigidBodyFlag::eKINEMATIC) == physx::PxRigidBodyFlag::eKINEMATIC) ? true : false;

	lua_pushboolean( state, enabled ? 1 : 0 );
	return 1;
}

static const luaL_Reg PxRigidBodyFunctions[] =
{
	{ "AttachTo", PxRigidBody_AttachTo },
	{ "SetDisableGravity", PxRigidBody_SetDisableGravity },
	{ "GetDisableGravity", PxRigidBody_GetDisableGravity },
	{ "SetKinematic", PxRigidBody_SetKinematic },
	{ "GetKinematic", PxRigidBody_GetKinematic },
};

int PxRigidBody_Constructor( lua_State * state )
{
	assert( 0 );
	/*
	std::string type = lua_tostring( state, 1 );

	ShapeParameters ** parameters = (ShapeParameters**)(lua_newuserdata( state, sizeof( ShapeParameters* ) ));
	*parameters = new ShapeParameters;
	(*parameters)->parameters.Set< shapes::Shape::TYPE >( "type",shapes::Shape::FromString( type ) );
	luaL_setmetatable( state, "ShapeParameters" );
	*/
	return 1;
}

int PxRigidBody_Destructor( lua_State * state )
{
	PxRigidBodyProxy * proxy = CheckPxRigidBody( state, 1 );
	delete proxy;
	return 0;
}

void RegisterPxRigidBody( dxilua::ScriptEngine * luaSE, me::IGame * game )
{
	g_luaSE = luaSE;

	luaSE->AddType( "PxRigidBody", PxRigidBodyFunctions, sizeof( PxRigidBodyFunctions ) / sizeof( luaL_Reg ), PxRigidBody_Constructor, PxRigidBody_Destructor );
}

