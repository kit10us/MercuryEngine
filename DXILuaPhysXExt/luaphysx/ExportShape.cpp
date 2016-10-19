// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/DXILua.h>
#include "luaphysx/ExportShape.h"

#include <dxilua/Matrix.h>
#include <dxilua/Color.h>
#include <dxilua/Size2.h>
#include <dxilua/Size3.h>
#include <dxilua/V2.h>
#include <dxilua/V3.h>

#include <PxPhysicsAPI.h>

using namespace dxilua;
using namespace dxi;

static dxilua::ScriptEngine * g_luaSE;

int PushPxShape( lua_State * state, std::shared_ptr< physx::PxShape > shape )
{
	PxShapeProxy ** newProxy = (PxShapeProxy**)(lua_newuserdata( state, sizeof( PxShapeProxy* ) ));
	*newProxy = new PxShapeProxy();
	(*newProxy)->shape = shape;
	luaL_setmetatable( state, "PxShape" );
	return 1;
}

PxShapeProxy* CheckPxShape( lua_State* state, int index )
{
	PxShapeProxy* ud = *(PxShapeProxy**)luaL_checkudata( state, index, "PxShape" );
	return ud;
}					 


int PxShape_CreateBox( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	auto v3 = CheckV3( state, 1 );
	/*

	std::shared_ptr< physx::PxShape > shape( 
	PxShape * cube = m_physics->createShape( PxBoxGeometry( v3.x, v3.y, v3.z ), *m_material );

	  */

	int x( 0 ); x;

	return 0;
}

static const luaL_Reg PxShapeFunctions[] =
{
	{ "CreateBox", PxShape_CreateBox },
	{ nullptr, nullptr }
};

int PxShape_Constructor( lua_State * state )
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

int PxShape_Destructor( lua_State * state )
{
	PxShapeProxy * proxy = CheckPxShape( state, 1 );
	delete proxy;
	return 0;
}

void RegisterPxShape( dxilua::ScriptEngine * luaSE )
{
	g_luaSE = luaSE;

	luaSE->AddLibrary( "PxShape", PxShapeFunctions, 1 );

	/*
	lua_register( state, "PxShape", Shape_Constructor );
	luaL_newmetatable( state, "PhysX" );
	lua_pushcfunction( state, Shape_Destructor ); lua_setfield( state, -2, "__gc" );
	lua_pushvalue( state, -1 ); lua_setfield( state, -2, "__index" );
	luaL_setfuncs( state, PhysXFunctions, 0 );
	lua_pop( state, 1 );
	*/
}

