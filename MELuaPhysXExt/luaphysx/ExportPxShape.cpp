// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <luaphysx/ExportPxShape.h>

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

int PushPxShape( lua_State * state, mephysx::objectcomponent::ColliderBase::ptr component )
{
	PxShapeProxy ** newProxy = (PxShapeProxy**)(lua_newuserdata( state, sizeof( PxShapeProxy* ) ));
	*newProxy = new PxShapeProxy();
	(*newProxy)->component = component;
	(*newProxy)->shape = dynamic_cast< mephysx::objectcomponent::ColliderBase * >( component.get() )->GetShape();
	luaL_setmetatable( state, "PxShape" );
	return 1;
}

PxShapeProxy* CheckPxShape( lua_State* state, int index )
{
	PxShapeProxy* ud = *(PxShapeProxy**)luaL_checkudata( state, index, "PxShape" );
	return ud;
}					 

int PxShape_AttachTo( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	PxShapeProxy * pxShape = CheckPxShape( state, 1 );
	ObjectProxy * object = CheckObject( state, 2 );

	object->object->AddComponent( pxShape->component );

	return 0;
}

static const luaL_Reg PxShapeFunctions[] =
{
	{ "AttachTo", PxShape_AttachTo }
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

void RegisterPxShape( melua::ScriptEngine * luaSE, me::IGame * game )
{
	g_luaSE = luaSE;

	luaSE->AddType( "PxShape", PxShapeFunctions, sizeof( PxShapeFunctions ) / sizeof( luaL_Reg ), PxShape_Constructor, PxShape_Destructor );
}

