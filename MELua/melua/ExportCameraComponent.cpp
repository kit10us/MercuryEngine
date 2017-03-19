// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <melua/ExportCameraComponent.h>
#include <melua/ExportObjectComponent.h>
#include <melua/ExportObject.h>
#include <melua/unify/ExportMatrix.h>
#include <melua/ScriptEngine.h>

#include <melua/unify/ExportMatrix.h>
#include <melua/unify/ExportColor.h>
#include <melua/unify/ExportSize2.h>
#include <melua/unify/ExportSize3.h>
#include <melua/unify/ExportV2.h>
#include <melua/unify/ExportV3.h>

using namespace melua;
using namespace me;
using namespace scene;
using namespace object;

CameraComponentProxy* CheckCameraComponent( lua_State* state, int index )
{
	CameraComponentProxy* ud = *(CameraComponentProxy**)luaL_checkudata( state, index, "CameraComponent" );
	return ud;
}

int PushCameraComponent( lua_State * state, me::object::ObjectComponent::ptr component )
{
	CameraComponentProxy ** proxy = (CameraComponentProxy**)(lua_newuserdata( state, sizeof( CameraComponentProxy* ) ));
	*proxy = new CameraComponentProxy;
	luaL_setmetatable( state, "CameraComponent" );
	(*proxy)->component = component;
	(*proxy)->camera = dynamic_cast< CameraComponent *>(component.get());
	return 1;
}														

int CameraComponent_SetProjection( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	CameraComponentProxy * componentProxy = CheckCameraComponent( state, 1 );
	
	unify::Matrix mat = CheckMatrix( state, 2 );

	componentProxy->camera->SetProjection( mat );

	return 0;
}

int CameraComponent_GetProjection( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	CameraComponentProxy * componentProxy = CheckCameraComponent( state, 1 );

	PushMatrix( state, componentProxy->camera->GetProjection() );

	return 1;
}

int CameraComponent_GetRenderer( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	CameraComponentProxy * componentProxy = CheckCameraComponent( state, 1 );

	lua_pushnumber( state, componentProxy->camera->GetRenderer() );

	return 1;
}

int CameraComponent_SetRenderer( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	CameraComponentProxy * componentProxy = CheckCameraComponent( state, 1 );
	int renderer = (int)lua_tonumber( state, 2 );

	componentProxy->camera->SetRenderer( renderer );

	return 0;
}

int CameraComponent_SetEnabled( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	CameraComponentProxy * componentProxy = CheckCameraComponent( state, 1 );
	bool enabled = lua_toboolean( state, 2 ) ? true : false;

	componentProxy->camera->SetEnabled( enabled );
	return 0;
}

int CameraComponent_IsEnabled( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	CameraComponentProxy * componentProxy = CheckCameraComponent( state, 1 );

	lua_pushboolean( state, componentProxy->camera->IsEnabled() );

	return 1;
}


static const luaL_Reg CameraComponentFunctions[] =
{
	{ "SetProjection", CameraComponent_SetProjection },
	{ "GetProjection", CameraComponent_GetProjection },
	{ "GetRenderer", CameraComponent_GetRenderer },
	{ "IsEnabled", CameraComponent_IsEnabled } ,
	{ "SetEnabled", CameraComponent_SetEnabled },
	{ "SetRenderer", CameraComponent_SetRenderer },
	{ nullptr, nullptr }
};

int CameraComponent_Constructor( lua_State * state )
{
	auto game = ScriptEngine::GetGame();

	int args = lua_gettop( state );

	if ( args == 0 )
	{
		IObjectComponent::ptr component( new CameraComponent() );
		return PushCameraComponent( state, component );
	}
	else if ( args == 1 )
	{
		ObjectComponentProxy * proxy = CheckObjectComponent( state, 1 );
		return PushCameraComponent( state, proxy->component );
	}
	else
	{
		lua_pushnil( state );
		return 1;
	}
}

int CameraComponent_Destructor( lua_State * state )
{
	CameraComponentProxy * proxy = CheckCameraComponent( state, 1 );
	delete proxy;
	return 0;
}

void RegisterCameraComponent( lua_State * state )
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	se->AddType( "CameraComponent", CameraComponentFunctions, sizeof( CameraComponentFunctions ) / sizeof( luaL_Reg ), CameraComponent_Constructor, CameraComponent_Destructor );
}

