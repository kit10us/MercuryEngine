// Copyright (c) 2002 - 2011, Evil Quail LLC
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <luaphysx/ExportPxController.h>

#include <melua/unify/ExportMatrix.h>
#include <melua/unify/ExportColor.h>
#include <melua/unify/ExportSize2.h>
#include <melua/unify/ExportSize3.h>
#include <melua/unify/ExportV2.h>
#include <melua/unify/ExportV3.h>
#include <melua/ExportObject.h>

#include <PxPhysicsAPI.h>

#include <me/Game.h>

using namespace melua;

static melua::ScriptEngine * g_luaSE;
static me::Game * g_game;

int PushPxController( lua_State * state, mephysx::objectcomponent::ControllerBase::ptr component )
{
	PxControllerProxy ** newProxy = (PxControllerProxy**)(lua_newuserdata( state, sizeof( PxControllerProxy* ) ));
	*newProxy = new PxControllerProxy();
	(*newProxy)->component = component;
	(*newProxy)->controller = dynamic_cast< mephysx::objectcomponent::ControllerBase * >( component.get() );
	luaL_setmetatable( state, "PxController" );

	return 1;
}

PxControllerProxy* CheckPxController( lua_State* state, int index )
{
	PxControllerProxy * proxy = *(PxControllerProxy**)luaL_checkudata( state, index, "PxController" );
	return proxy;
}					 
							  
int PxController_AttachTo( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	PxControllerProxy * proxy = CheckPxController( state, 1 );
	ObjectProxy * object = CheckObject( state, 2 );

	object->object->AddComponent( proxy->component );

	return 0;
}

static const luaL_Reg PxControllerFunctions[] =
{
	{ "AttachTo", PxController_AttachTo },
};

int PxController_Constructor( lua_State * state )
{
	assert( 0 );
	return 1;
}

int PxController_Destructor( lua_State * state )
{
	PxControllerProxy * proxy = CheckPxController( state, 1 );
	delete proxy;
	return 0;
}

void RegisterPxController( melua::ScriptEngine * luaSE, me::IGame * game )
{
	g_luaSE = luaSE;

	luaSE->AddType( { "PxController", PxControllerFunctions, sizeof( PxControllerFunctions ) / sizeof( luaL_Reg ), PxController_Constructor, PxController_Destructor } );
}

