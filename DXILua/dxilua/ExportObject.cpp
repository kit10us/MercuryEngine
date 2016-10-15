// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/DXILua.h>
#include <dxilua/ExportObject.h>
#include <dxilua/ExportTransform.h>
#include <dxilua/ExportScene.h>
#include <dxilua/unify/ExportV2.h>
#include <dxilua/unify/ExportV3.h>
#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/ExportCameraComponent.h>
#include <dxi/scene/ScriptComponent.h>
#include <dxilua/ExportGeometry.h>

using namespace dxilua;
using namespace dxi;

static size_t g_objectCount = 0;

int PushObject( lua_State * state, dxi::scene::Object::ptr object )
{
	ObjectProxy ** childProxy = (ObjectProxy**)(lua_newuserdata( state, sizeof( ObjectProxy* ) ));
	*childProxy = new ObjectProxy;
	luaL_setmetatable( state, "Object" );
	(*childProxy)->object = object;
	g_objectCount++;
	return 1;
}

ObjectProxy* CheckObject( lua_State* state, int index )
{
	ObjectProxy* ud = *(ObjectProxy**)luaL_checkudata( state, index, "Object" );
	return ud;
}
			   
int Object_AddChild( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );
	std::string name = lua_tostring( state, 2 );

	dxi::scene::Object::ptr child = objectProxy->object->AddChild( name );

	ObjectProxy ** childProxy = (ObjectProxy**)(lua_newuserdata( state, sizeof( ObjectProxy* ) ));
	*childProxy = new ObjectProxy;
	luaL_setmetatable( state, "Object" );
	(*childProxy)->object = child;

	return 1;
}

int Object_AddCamera( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );
	std::string name = lua_tostring( state, 2 );

	int t = lua_type( state, 3 );
	unify::Matrix mat = CheckMatrix( state, 3 );

	dxi::scene::Object::ptr child = objectProxy->object->AddChild( name );
	dxi::scene::Camera * cameraComponent = new scene::Camera;
	child->AddComponent( scene::IComponent::ptr( cameraComponent ) );

	cameraComponent->SetProjection( mat );

	ObjectProxy ** childProxy = (ObjectProxy**)(lua_newuserdata( state, sizeof( ObjectProxy* ) ));
	*childProxy = new ObjectProxy;
	luaL_setmetatable( state, "Object" );
	(*childProxy)->object = child;

	return 1;
}

int Object_AddScript( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );
	std::string name = lua_tostring( state, 2 );
	std::string type = lua_tostring( state, 3 );
	unify::Path source = lua_tostring( state, 4 );

	dxi::scene::ScriptComponent * component = new scene::ScriptComponent();

	auto game = ScriptEngine::GetGame();
	auto se = game->GetScriptEngine( type );
	assert( se ); //TODO: Handle error better.

	scripting::IModule::ptr module = se->LoadModule( source, objectProxy->object );

	component->SetModule( module );

	objectProxy->object->AddComponent( scene::IComponent::ptr( component ) );

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

int Object_SetSelectable( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );
	bool selectable = lua_toboolean( state, 2 ) ? true : false;

	objectProxy->object->SetSelectable( selectable );
	return 0;
}

int Object_IsSelectable( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );

	lua_pushboolean( state, objectProxy->object->GetSelectable() );

	return 1;
}

int Object_SetGeometry( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );
							
	Geometry::ptr geometry;
	
	// If string, pull an existing resource...
	if ( lua_type( state, 2 ) == LUA_TSTRING ) 
	{
		std::string geometryName = lua_tostring( state, 2 );
		auto game = ScriptEngine::GetGame();
		geometry = game->GetManager< Geometry >()->Find( geometryName );
	}
	else
	{
		geometry = CheckGeometry( state, 2 )->geometry;
	}

	objectProxy->object->SetGeometry( geometry );

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

int Object_GetComponent( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );
	std::string name = lua_tostring( state, 2 );

	dxi::scene::IComponent::ptr component = objectProxy->object->GetComponent( name );

	if ( ! component )
	{
		lua_pushnil( state );
		return 1;
	}

	// Camera..
	dxi::scene::Camera * cameraComponent = dynamic_cast<dxi::scene::Camera *>( component.get() );
	if ( cameraComponent )
	{
		CameraComponentProxy ** proxy = (CameraComponentProxy**)(lua_newuserdata( state, sizeof( CameraComponentProxy* ) ));
		*proxy = new CameraComponentProxy;
		luaL_setmetatable( state, "CameraComponent" );
		(*proxy)->camera = cameraComponent; 
	}

	return 1;
}

static const luaL_Reg ObjectFunctions[] =
{
	{ "AddChild", Object_AddChild },
	{ "AddCamera", Object_AddCamera },
	{ "AddScript", Object_AddScript },
	{ "Name", Object_Name },
	{ "SetEnabled", Object_SetEnabled },
	{ "GetEnabled", Object_IsEnabled },
	{ "SetSelectable", Object_SetSelectable },
	{ "GetSelectable", Object_IsSelectable },
	{ "SetGeometry", Object_SetGeometry },
	{ "GetSize", Object_GetSize },
	{ "Transform", Object_Transform },
	{ "GetComponent", Object_GetComponent },
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
	g_objectCount--;
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

