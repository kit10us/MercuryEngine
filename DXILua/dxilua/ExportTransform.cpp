// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/DXILua.h>
#include <dxilua/ExportTransform.h>
#include <dxilua/ExportObject.h>
#include <dxilua/ExportScene.h>
#include <dxilua/unify/ExportV2.h>
#include <dxilua/unify/ExportV3.h>
#include <dxilua/unify/ExportMatrix.h>
#include <dxi/scene/ScriptComponent.h>

#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/unify/ExportColor.h>
#include <dxilua/unify/ExportSize2.h>
#include <dxilua/unify/ExportSize3.h>
#include <dxilua/unify/ExportV2.h>
#include <dxilua/unify/ExportV3.h>
#include <dxilua/unify/ExportQuaternion.h>

using namespace dxilua;
using namespace dxi;

TransformProxy* CheckTransform( lua_State* state, int index )
{
	TransformProxy* ud = *(TransformProxy**)luaL_checkudata( state, index, "Transform" );
	return ud;
}
			   
int Transform_SetPosition( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	TransformProxy * proxy = CheckTransform( state, 1 );

	unify::V3< float > position( CheckV3( state, 2 ) );

	auto game = ScriptEngine::GetGame();

	proxy->transform->SetPosition( position );

	return 0;
}

int Transform_LookAt( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	TransformProxy * proxy = CheckTransform( state, 1 );

	unify::V3< float > position( CheckV3( state, 2 ) );

	auto game = ScriptEngine::GetGame();

	proxy->transform->LookAt( position );

	return 0;
}

int Transform_Orbit( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	TransformProxy * proxy = CheckTransform( state, 1 );

	unify::V3< float > origin( CheckV3( state, 2 ) );

	unify::V2< float > axis( CheckV2( state, 3 ) );

	float distance = (float)lua_tonumber( state, 4 );
	
	proxy->transform->Orbit( origin, axis, unify::AngleInRadians( distance ) );

	return 0;
}

int Transform_RotateAbout( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	TransformProxy * proxy = CheckTransform( state, 1 );

	unify::V3< float > axis( CheckV3( state, 2 ) );

	float rotation = (float)lua_tonumber( state, 3 );

	proxy->transform->PreMul( unify::Quaternion( axis, unify::AngleInRadians( rotation ) ) );

	return 0;
}

int Transform_Scale( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectProxy * objectProxy = CheckObject( state, 1 );
	float scale = (float)lua_tonumber( state, 2 );

	auto x = objectProxy->object->GetComponent( "Geometry" ).get();
	scene::GeometryComponent * geometry = unify::polymorphic_downcast< scene::GeometryComponent * >( x );

	geometry->GetModelMatrix().Scale( scale );
	return 0;
}

int Transform_GetRotation( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	TransformProxy * proxy = CheckTransform( state, 1 );

	PushQuaternion( state, proxy->transform->GetRotation() );

	return 1;
}

int Transform_SetRotation( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	TransformProxy * proxy = CheckTransform( state, 1 );

	unify::Quaternion q = CheckQuaternion( state, 2 );

	proxy->transform->SetRotation( q );

	return 0;
}

int Transform_PreMul( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	TransformProxy * proxy = CheckTransform( state, 1 );
	unify::Quaternion q = CheckQuaternion( state, 2 );
	
	unify::Quaternion myQ = proxy->transform->GetRotation();

	proxy->transform->SetRotation( q * myQ );

	return 0;
}

int Transform_PostMul( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	TransformProxy * proxy = CheckTransform( state, 1 );
	unify::Quaternion q = CheckQuaternion( state, 2 );

	unify::Quaternion myQ = proxy->transform->GetRotation();

	proxy->transform->SetRotation( myQ * q );

	return 0;
}


static const luaL_Reg TransformFunctions[] =
{
	{ "SetPosition", Transform_SetPosition },
	{ "LookAt", Transform_LookAt },
	{ "Orbit", Transform_Orbit },
	{ "RotateAbout", Transform_RotateAbout },
	{ "Scale", Transform_Scale },
	{ "GetRotation", Transform_GetRotation },
	{ "SetRotation", Transform_SetRotation },
	{ "PreMul", Transform_PreMul },
	{ "PostMul", Transform_PostMul },
	{ nullptr, nullptr }
};

int Transform_Constructor( lua_State * state )
{
	/*
	SceneProxy * sceneProxy = CheckScene( state, -1 );

	std::string name = luaL_checkstring( state, -2 );
	TransformProxy ** objectProxy = (TransformProxy**)(lua_newuserdata( state, sizeof( TransformProxy* ) ));
	*objectProxy = new TransformProxy;
	luaL_setmetatable( state, "Transform" );

	auto game = ScriptEngine::GetGame();

	(*poxy)->object = sceneProxy->scene->GetRoot()->AddChild( name );
	*/
	assert( 0 );  // TODO: Can support this, once I clean up the Transform Proxy to support shared_ptr as as well as straight ptrs.
	return 0;
}

int Transform_Destructor( lua_State * state )
{
	TransformProxy * proxy = CheckTransform( state, 1 );
	delete proxy;
	return 0;
}

void RegisterTransform( lua_State * state )
{
	lua_register( state, "Transform", Transform_Constructor );
	luaL_newmetatable( state, "Transform" );
	lua_pushcfunction( state, Transform_Destructor ); lua_setfield( state, -2, "__gc" );
	lua_pushvalue( state, -1 ); lua_setfield( state, -2, "__index" );
	luaL_setfuncs( state, TransformFunctions, 0 );
	lua_pop( state, 1 );
}

