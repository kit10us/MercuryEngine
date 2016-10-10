// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ExportCameraComponent.h>
#include <dxilua/ExportMatrix.h>
#include <dxilua/DXILua.h>

using namespace dxilua;
using namespace dxi;

CameraComponentProxy* CheckCameraComponent( lua_State* state, int index )
{
	CameraComponentProxy* ud = *(CameraComponentProxy**)luaL_checkudata( state, index, "CameraComponent" );
	return ud;
}

int CameraComponent_SetName( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	CameraComponentProxy * componentProxy = CheckCameraComponent( state, 1 );
	std::string name = lua_tostring( state, 2 );

	componentProxy->camera->SetName( name );

	return 0;
}

int CameraComponent_Name( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	CameraComponentProxy * componentProxy = CheckCameraComponent( state, 1 );
	lua_pushstring( state, componentProxy->camera->GetName().c_str() );

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
	{ "SetName", CameraComponent_SetName },
	{ "Name", CameraComponent_Name },
	{ "GetRenderer", CameraComponent_GetRenderer },
	{ "IsEnabled", CameraComponent_IsEnabled } ,
	{ "SetEnabled", CameraComponent_SetEnabled },
	{ "SetName", CameraComponent_SetName },
	{ "SetRenderer", CameraComponent_SetRenderer },
	{ nullptr, nullptr }
};

int CameraComponent_Constructor( lua_State * state )
{
	ScriptEngine::GetGame()->ReportError( dxi::ErrorLevel::Failure, "LUA", "No support for creating new cameras directly!" );
	return 0;
}

int CameraComponent_Destructor( lua_State * state )
{
	CameraComponentProxy * proxy = CheckCameraComponent( state, 1 );
	delete proxy;
	return 0;
}

void RegisterCameraComponent( lua_State * state )
{
	lua_register( state, "CameraComponent", CameraComponent_Constructor );
	luaL_newmetatable( state, "CameraComponent" );
	lua_pushcfunction( state, CameraComponent_Destructor ); lua_setfield( state, -2, "__gc" );
	lua_pushvalue( state, -1 ); lua_setfield( state, -2, "__index" );
	luaL_setfuncs( state, CameraComponentFunctions, 0 );
	lua_pop( state, 1 );
}

