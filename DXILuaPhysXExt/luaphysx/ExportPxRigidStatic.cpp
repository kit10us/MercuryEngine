// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ScriptEngine.h>
#include <luaphysx/ExportPxRigidStatic.h>

#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/unify/ExportColor.h>
#include <dxilua/unify/ExportSize2.h>
#include <dxilua/unify/ExportSize3.h>
#include <dxilua/unify/ExportV2.h>
#include <dxilua/unify/ExportV3.h>
#include <dxilua/ExportObject.h>

#include <PxPhysicsAPI.h>

#include <dxi/core/Game.h>

using namespace dxilua;
using namespace dxi;

static dxilua::ScriptEngine * g_luaSE;
static dxi::core::Game * g_game;

int PushPxRigidStatic( lua_State * state, dxiphysx::objectcomponent::RigidStatic::ptr component )
{
	PxRigidStaticProxy ** newProxy = (PxRigidStaticProxy**)(lua_newuserdata( state, sizeof( PxRigidStaticProxy* ) ));
	*newProxy = new PxRigidStaticProxy();
	(*newProxy)->component = component;
	(*newProxy)->body = dynamic_cast< dxiphysx::objectcomponent::RigidStatic * >( component.get() )->GetRigidStatic();
	luaL_setmetatable( state, "PxRigidStatic" );

	return 1;
}

PxRigidStaticProxy* CheckPxRigidStatic( lua_State* state, int index )
{
	PxRigidStaticProxy * ud = *(PxRigidStaticProxy**)luaL_checkudata( state, index, "PxRigidStatic" );
	return ud;
}					 
							  
int PxRigidStatic_AttachTo( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	PxRigidStaticProxy * pxBody = CheckPxRigidStatic( state, 1 );
	ObjectProxy * object = CheckObject( state, 2 );

	object->object->AddComponent( pxBody->component );

	return 0;
}

int PxRigidStatic_SetDisableGravity( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	PxRigidStaticProxy * pxBody = CheckPxRigidStatic( state, 1 );
	bool disableGravity = lua_toboolean( state, 2 ) ? true : false;
	
	pxBody->body->setActorFlag( physx::PxActorFlag::eDISABLE_GRAVITY, disableGravity );

	return 0;
}

int PxRigidStatic_GetDisableGravity( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	PxRigidStaticProxy * pxBody = CheckPxRigidStatic( state, 1 );
	bool disableGravity = ((pxBody->body->getActorFlags() & physx::PxActorFlag::eDISABLE_GRAVITY) == physx::PxActorFlag::eDISABLE_GRAVITY) ? true : false;

	lua_pushboolean( state, disableGravity ? 1 : 0 );
	return 1;
}

static const luaL_Reg PxRigidStaticFunctions[] =
{
	{ "AttachTo", PxRigidStatic_AttachTo },
	{ "SetDisableGravity", PxRigidStatic_SetDisableGravity },
	{ "GetDisableGravity", PxRigidStatic_GetDisableGravity },
};

int PxRigidStatic_Constructor( lua_State * state )
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

int PxRigidStatic_Destructor( lua_State * state )
{
	PxRigidStaticProxy * proxy = CheckPxRigidStatic( state, 1 );
	delete proxy;
	return 0;
}

void RegisterPxRigidStatic( dxilua::ScriptEngine * luaSE, dxi::core::IGame * game )
{
	g_luaSE = luaSE;

	luaSE->AddType( "PxRigidStatic", PxRigidStaticFunctions, sizeof( PxRigidStaticFunctions ) / sizeof( luaL_Reg ), PxRigidStatic_Constructor, PxRigidStatic_Destructor );
}

