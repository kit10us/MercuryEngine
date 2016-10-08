// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/DXILua.h>
#include <dxilua/ExportObject.h>
#include <dxilua/ExportScene.h>
#include <dxilua/ExportV2.h>
#include <dxilua/ExportV3.h>
#include <dxilua/ExportMatrix.h>

using namespace dxilua;
using namespace dxi;

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

int Object_GetEnabled( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );
		 
	lua_pushboolean( state, objectProxy->object->GetEnabled() );

	return 1;
}

int Object_SetVisible( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );
	bool visible = lua_toboolean( state, 2 ) ? true : false;

	objectProxy->object->SetVisible( visible );
	return 0;
}

int Object_GetVisible( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );

	lua_pushboolean( state, objectProxy->object->GetVisible() );

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

int Object_GetSelectable( lua_State * state )
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
																	
	std::string geometryName = lua_tostring( state, 2 );

	auto game = ScriptEngine::GetGame();

	auto geometry = game->GetManager< Geometry >()->Find( geometryName );

	objectProxy->object->SetGeometry( geometry );

	return 0;
}

int Object_SetPosition( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );

	unify::V3< float > position( lua_ToV3( state, 2 ) );

	auto game = ScriptEngine::GetGame();

	objectProxy->object->GetFrame().SetPosition( position );

	return 0;
}

int Object_LookAt( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );

	unify::V3< float > position( lua_ToV3( state, 2 ) );

	auto game = ScriptEngine::GetGame();

	objectProxy->object->GetFrame().LookAt( position );

	return 0;
}

int Object_Orbit( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );

	unify::V3< float > origin( lua_ToV3( state, 2 ) );

	unify::V2< float > axis( lua_ToV2( state, 3 ) );

	float distance = (float)lua_tonumber( state, 4 );
	
	objectProxy->object->GetFrame().Orbit( origin, axis, unify::Angle::AngleInRadians( distance ) );

	return 0;
}

int Object_RotateAbout( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );

	unify::V3< float > axis( lua_ToV3( state, 2 ) );

	float rotation = (float)lua_tonumber( state, 3 );

	objectProxy->object->GetFrame().RotateAbout( axis, unify::Angle::AngleInRadians( rotation ) );

	return 0;
}

int Object_GetSize( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );

	lua_pushnumber( state, objectProxy->object->GetBBox().Size().Length() );

	return 1;
}

int Object_Scale( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );
	float scale = (float)lua_tonumber( state, 2 );

	objectProxy->object->GetGeometryMatrix().Scale( scale );
	return 0;
}

static const luaL_Reg ObjectFunctions[] =
{
	{ "AddChild", Object_AddChild },
	{ "AddCamera", Object_AddCamera },
	{ "Name", Object_Name },
	{ "SetEnabled", Object_SetEnabled },
	{ "GetEnabled", Object_GetEnabled },
	{ "SetVisible", Object_SetVisible },
	{ "GetVisible", Object_GetVisible },
	{ "SetSelectable", Object_SetSelectable },
	{ "GetSelectable", Object_GetSelectable },
	{ "SetGeometry", Object_SetGeometry },
	{ "SetPosition", Object_SetPosition },
	{ "LookAt", Object_LookAt },
	{ "Orbit", Object_Orbit },
	{ "RotateAbout", Object_RotateAbout },
	{ "GetSize", Object_GetSize },
	{ "Scale", Object_Scale },
	{ nullptr, nullptr }
};

int Object_Constructor( lua_State * state )
{
	SceneProxy * sceneProxy = CheckScene( state, -1 );

	std::string name = luaL_checkstring( state, -2 );
	ObjectProxy ** objectProxy = (ObjectProxy**)(lua_newuserdata( state, sizeof( ObjectProxy* ) ));
	*objectProxy = new ObjectProxy;
	luaL_setmetatable( state, "Object" );

	auto game = ScriptEngine::GetGame();

	(*objectProxy)->object = sceneProxy->scene->GetRoot()->AddChild( name );
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

