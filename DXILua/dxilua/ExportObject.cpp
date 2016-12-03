// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>

#include <dxilua/ScriptEngine.h>
#include <dxilua/ExportObject.h>
#include <dxilua/ExportTransform.h>
#include <dxilua/ExportScene.h>
#include <dxilua/unify/ExportV2.h>
#include <dxilua/unify/ExportV3.h>
#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/ExportCameraComponent.h>
#include <dxilua/ExportGeometry.h>
#include <dxilua/ExportTerra.h>
#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/unify/ExportColor.h>
#include <dxilua/unify/ExportSize2.h>
#include <dxilua/unify/ExportSize3.h>
#include <dxilua/unify/ExportV2.h>
#include <dxilua/unify/ExportV3.h>
#include <me/scene/GeometryComponent.h>

using namespace dxilua;
using namespace me;
using namespace scene;

int PushObject( lua_State * state, me::scene::Object * object )
{
	ObjectProxy ** childProxy = (ObjectProxy**)(lua_newuserdata( state, sizeof( ObjectProxy* ) ));
	*childProxy = new ObjectProxy;
	luaL_setmetatable( state, "Object" );
	(*childProxy)->object = object;
	return 1;
}

ObjectProxy* CheckObject( lua_State* state, int index )
{
	ObjectProxy* ud = *(ObjectProxy**)luaL_checkudata( state, index, "Object" );
	return ud;
}

int Object_AddScript( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );
	std::string name = lua_tostring( state, 2 );
	std::string type = lua_tostring( state, 3 );
	unify::Path source = lua_tostring( state, 4 );

	auto game = ScriptEngine::GetGame();

	//dxi::scene::ScriptComponent * component = new scene::ScriptComponent( game->GetOS() );

	auto gcse = game->GetComponent( type, 0 );
	if( !gcse ) game->ReportError( ErrorLevel::Failure, "Lua", "Could not find " + type + " script engine!" );
	IScriptEngine * se = dynamic_cast< IScriptEngine *>(gcse.get() );

	auto component = se->LoadModule( source );

	objectProxy->object->AddComponent( IObjectComponent::ptr( component ) );

	return 0;
}

int Object_Name( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );

	lua_pushstring( state, objectProxy->object->GetName().c_str() );

	return 1;
}

int Object_SetEnabled( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );
	bool enabled = lua_toboolean( state, 2 ) ? true : false;
	
	objectProxy->object->SetEnabled( enabled );
	return 0;
}

int Object_IsEnabled( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );
		 
	lua_pushboolean( state, objectProxy->object->IsEnabled() );

	return 1;
}

int Object_SetGeometry( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );
							
	auto game = dynamic_cast< me::Game * >(ScriptEngine::GetGame());

	Geometry::ptr geometry;
	
	// If string, pull an existing resource...
	if ( lua_type( state, 2 ) == LUA_TSTRING ) 
	{
		std::string geometryName = lua_tostring( state, 2 );
		geometry = game->GetManager< Geometry >()->Find( geometryName );
	}
	else
	{
		GeometryProxy * geometryProxy = CheckGeometry( state, 2 );
		if ( geometryProxy )
		{
			geometry = geometryProxy->geometry;
		}
		else
		{
			TerraProxy * terraProxy = CheckTerra( state, 2 );
			geometry = terraProxy->geometry;
		}
	}

	AddGeometryComponent( objectProxy->object, game->GetOS(), geometry );

	return 0;
}

int Object_GetSize( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );

	// TODO:
	lua_pushnumber( state, /*objectProxy->object->GetBBox().Size().Length()*/ 1.0f );

	return 1;
}

int Object_Transform( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );

	TransformProxy ** childProxy = (TransformProxy**)(lua_newuserdata( state, sizeof( TransformProxy* ) ));
	*childProxy = new TransformProxy;
	luaL_setmetatable( state, "Transform" );
	(*childProxy)->object = objectProxy->object;
	(*childProxy)->transform = &objectProxy->object->GetFrame();
	return 1;
}

int Object_AddTag( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );
	std::string tag( lua_tostring( state, 2 ) );

	objectProxy->object->AddTag( tag );

	return 1;
}

int Object_HasTag( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );
	std::string tag( lua_tostring( state, 2 ) );

	lua_pushboolean( state, objectProxy->object->HasTag( tag ) ? 1 : 0 );

	return 1;
}

int Object_GetComponent( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );
	std::string name = lua_tostring( state, 2 );

	IObjectComponent::ptr component = objectProxy->object->GetComponent( name );

	if ( ! component )
	{
		lua_pushnil( state );
		return 1;
	}

	// Camera..
	CameraComponent * camera = dynamic_cast< CameraComponent * >( component.get() );
	if ( camera )
	{
		CameraComponentProxy ** proxy = (CameraComponentProxy**)(lua_newuserdata( state, sizeof( CameraComponentProxy* ) ));
		*proxy = new CameraComponentProxy;
		luaL_setmetatable( state, "CameraComponent" );
		(*proxy)->camera = camera;
		(*proxy)->component = component;
	}

	return 1;
}

static const luaL_Reg ObjectFunctions[] =
{
	{ "AddScript", Object_AddScript },
	{ "Name", Object_Name },
	{ "SetEnabled", Object_SetEnabled },
	{ "GetEnabled", Object_IsEnabled },
	{ "SetGeometry", Object_SetGeometry },
	{ "GetSize", Object_GetSize },
	{ "Transform", Object_Transform },
	{ "GetComponent", Object_GetComponent },
	{ "AddTag", Object_AddTag },
	{ "HasTag", Object_HasTag },
	{ nullptr, nullptr }
};

int Object_Constructor( lua_State * state )
{
	assert( 0 ); // NOT SUPPORTED.

	/*
	SceneProxy * sceneProxy = CheckScene( state, -1 );

	std::string name = luaL_checkstring( state, -2 );
	ObjectProxy ** objectProxy = (ObjectProxy**)(lua_newuserdata( state, sizeof( ObjectProxy* ) ));
	*objectProxy = new ObjectProxy;
	luaL_setmetatable( state, "Object" );

	auto game = ScriptEngine::GetGame();

	(*objectProxy)->object = sceneProxy->scene->GetRoot()->AddChild( name );
	*/
	return 1;
}

int Object_Destructor( lua_State * state )
{
	ObjectProxy * ObjectProxy = CheckObject( state, 1 );
	delete ObjectProxy;
	return 0;
}

void RegisterObject( lua_State * state )
{
	lua_register( state, "Object", Object_Constructor );
	luaL_newmetatable( state, "Object" );
	lua_pushcfunction( state, Object_Destructor ); lua_setfield( state, -2, "__gc" );
	lua_pushvalue( state, -1 ); lua_setfield( state, -2, "__index" );
	luaL_setfuncs( state, ObjectFunctions, 0 );
	lua_pop( state, 1 );
}

