// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/exports/ExportCameraComponent.h>
#include <melua/exports/ExportObjectComponent.h>
#include <melua/exports/ExportObject.h>
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
	
	unify::Matrix mat = CheckUserType< MatrixProxy >( state, 2 )->matrix;

	componentProxy->camera->SetProjection( mat );

	return 0;
}

int CameraComponent_GetProjection( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	CameraComponentProxy * componentProxy = CheckCameraComponent( state, 1 );

	return Push< MatrixProxy >( state, { componentProxy->camera->GetProjection() } );
}

int CameraComponent_GetRenderer( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	CameraComponentProxy * componentProxy = CheckCameraComponent( state, 1 );

	return Push( state, componentProxy->camera->GetRenderer() );
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

int CameraComponent_Constructor( lua_State * state )
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();

	int args = lua_gettop( state );

	if ( args == 0 )
	{
		IObjectComponent::ptr component( new CameraComponent() );
		return PushCameraComponent( state, component );
	}
	else if ( args == 1 )
	{
		ObjectComponentProxy * proxy = CheckUserType< ObjectComponentProxy >( state, 1 );
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

	static const luaL_Reg CameraComponentFunctions[] =
	{
		{ "SetProjection", CameraComponent_SetProjection },
		{ "GetProjection", CameraComponent_GetProjection },
		{ "GetRenderer", CameraComponent_GetRenderer },
		{ "IsEnabled", CameraComponent_IsEnabled },
		{ "SetEnabled", CameraComponent_SetEnabled },
		{ "SetRenderer", CameraComponent_SetRenderer },
		{ nullptr, nullptr }
	};

	Type type = { "CameraComponent", CameraComponentFunctions, CameraComponent_Constructor, CameraComponent_Destructor };
	se->AddType( type );
}

