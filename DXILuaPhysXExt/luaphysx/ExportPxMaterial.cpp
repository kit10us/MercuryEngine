// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/DXILua.h>
#include "luaphysx/ExportPxMaterial.h"

#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/unify/ExportColor.h>
#include <dxilua/unify/ExportSize2.h>
#include <dxilua/unify/ExportSize3.h>
#include <dxilua/unify/ExportV2.h>
#include <dxilua/unify/ExportV3.h>

#include <PxPhysicsAPI.h>

using namespace dxilua;
using namespace dxi;

static dxilua::ScriptEngine * g_luaSE;
static dxi::core::Game * g_game;

int PushPxMaterial( lua_State * state, std::shared_ptr< physx::PxMaterial > material )
{
	PxMaterialProxy ** newProxy = (PxMaterialProxy**)(lua_newuserdata( state, sizeof( PxMaterialProxy* ) ));
	*newProxy = new PxMaterialProxy();
	(*newProxy)->material = material;
	luaL_setmetatable( state, "PxMaterial" );
	return 1;
}

PxMaterialProxy* CheckPxMaterial( lua_State* state, int index )
{
	PxMaterialProxy* ud = *(PxMaterialProxy**)luaL_checkudata( state, index, "PxMaterial" );
	return ud;
}					 


int PxMaterial_Create( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 1 );
	/*

	std::shared_ptr< physx::PxMaterial > material ( m_physics->createMaterial( 0.5f, 0.5f, 0.6f ), Releaser< PxMaterial > );

	std::shared_ptr< physx::PxMaterial > shape( 
	PxMaterial * cube = m_physics->createShape( PxBoxGeometry( v3.x, v3.y, v3.z ), *m_material );	 
	*/

	int x( 0 ); x;

	return 0;
}

static const luaL_Reg PxMaterialFunctions[] =
{
	{ "Create", PxMaterial_Create },
	{ nullptr, nullptr }
};

int PxMaterial_Constructor( lua_State * state )
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

int PxMaterial_Destructor( lua_State * state )
{
	PxMaterialProxy * proxy = CheckPxMaterial( state, 1 );
	delete proxy;
	return 0;
}

void RegisterPxMaterial( dxilua::ScriptEngine * luaSE, dxi::core::Game * game )
{
	g_luaSE = luaSE;
	g_game = game;

	luaSE->AddLibrary( "PxMaterial", PxMaterialFunctions, 1 );

	/*
	lua_register( state, "PxMaterial", Shape_Constructor );
	luaL_newmetatable( state, "PhysX" );
	lua_pushcfunction( state, Shape_Destructor ); lua_setfield( state, -2, "__gc" );
	lua_pushvalue( state, -1 ); lua_setfield( state, -2, "__index" );
	luaL_setfuncs( state, PhysXFunctions, 0 );
	lua_pop( state, 1 );
	*/
}

