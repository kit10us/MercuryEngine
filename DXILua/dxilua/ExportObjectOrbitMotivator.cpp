// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ExportObjectOrbitMotivator.h>
#include <dxilua/ExportObject.h>
#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/ScriptEngine.h>

#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/unify/ExportColor.h>
#include <dxilua/unify/ExportSize2.h>
#include <dxilua/unify/ExportSize3.h>
#include <dxilua/unify/ExportV2.h>
#include <dxilua/unify/ExportV3.h>

using namespace dxilua;
using namespace me;
using namespace scene;
using namespace motivator;

ObjectOrbitMotivatorProxy* CheckObjectOrbitMotivator( lua_State* state, int index )
{
	ObjectOrbitMotivatorProxy* ud = *(ObjectOrbitMotivatorProxy**)luaL_checkudata( state, index, "ObjectOrbitMotivator" );
	return ud;
}

int PushObjectOrbitMotivator( lua_State * state, me::scene::ObjectComponent::ptr component )
{
	ObjectOrbitMotivatorProxy ** proxy = (ObjectOrbitMotivatorProxy**)(lua_newuserdata( state, sizeof( ObjectOrbitMotivatorProxy* ) ));
	*proxy = new ObjectOrbitMotivatorProxy;
	luaL_setmetatable( state, "ObjectOrbitMotivator" );
	(*proxy)->component = component;
	(*proxy)->motivator = dynamic_cast< ObjectOrbitMotivator *>(component.get());
	return 1;
}														

int ObjectOrbitMotivator_AttachTo( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectOrbitMotivatorProxy * cameraComponentProxy = CheckObjectOrbitMotivator( state, 1 );
	ObjectProxy * objectProxy = CheckObject( state, 2 );
	objectProxy->object->AddComponent( cameraComponentProxy->component );
	return 0;
}

int ObjectOrbitMotivator_Name( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectOrbitMotivatorProxy * motivatorProxy = CheckObjectOrbitMotivator( state, 1 );
	lua_pushstring( state, motivatorProxy->motivator->GetName().c_str() );

	return 1;
}

int ObjectOrbitMotivator_SetOrigin( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectOrbitMotivatorProxy * motivatorProxy = CheckObjectOrbitMotivator( state, 1 );
	
	unify::V3< float > v = CheckV3( state, 2 );

	motivatorProxy->motivator->SetOrigin( v );

	return 0;
}

int ObjectOrbitMotivator_GetOrigin( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectOrbitMotivatorProxy * motivatorProxy = CheckObjectOrbitMotivator( state, 1 );

	PushV3( state, motivatorProxy->motivator->GetOrigin() );

	return 1;
}

int ObjectOrbitMotivator_SetOrbit( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectOrbitMotivatorProxy * motivatorProxy = CheckObjectOrbitMotivator( state, 1 );
	
	unify::V3< float > v = CheckV3( state, 2 );

	motivatorProxy->motivator->SetOrbit( v );

	return 0;
}

int ObjectOrbitMotivator_GetOrbit( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectOrbitMotivatorProxy * motivatorProxy = CheckObjectOrbitMotivator( state, 1 );

	PushV3( state, motivatorProxy->motivator->GetOrbit() );

	return 1;
}

int ObjectOrbitMotivator_SetAngleASecond( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectOrbitMotivatorProxy * motivatorProxy = CheckObjectOrbitMotivator( state, 1 );
	
	unify::Angle angle( unify::AngleInRadians( (float)luaL_checknumber( state, 2 ) ) );

	motivatorProxy->motivator->SetAngleASecond( angle );

	return 0;
}

int ObjectOrbitMotivator_GetDegreesASecond( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectOrbitMotivatorProxy * motivatorProxy = CheckObjectOrbitMotivator( state, 1 );

	lua_pushnumber( state, motivatorProxy->motivator->GetAngleASecond().ToDegrees() );

	return 1;
}

int ObjectOrbitMotivator_GetRadiansASecond( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectOrbitMotivatorProxy * motivatorProxy = CheckObjectOrbitMotivator( state, 1 );

	lua_pushnumber( state, motivatorProxy->motivator->GetAngleASecond().ToRadians() );

	return 1;
}

int ObjectOrbitMotivator_SetEnabled( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectOrbitMotivatorProxy * motivatorProxy = CheckObjectOrbitMotivator( state, 1 );
	bool enabled = lua_toboolean( state, 2 ) ? true : false;

	motivatorProxy->motivator->SetEnabled( enabled );
	return 0;
}

int ObjectOrbitMotivator_IsEnabled( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectOrbitMotivatorProxy * motivatorProxy = CheckObjectOrbitMotivator( state, 1 );

	lua_pushboolean( state, motivatorProxy->motivator->IsEnabled() );

	return 1;
}


static const luaL_Reg ObjectOrbitMotivatorFunctions[] =
{
	{ "AttachTo", ObjectOrbitMotivator_AttachTo },
	{ "Name", ObjectOrbitMotivator_Name },
	{ "SetOrigin", ObjectOrbitMotivator_SetOrigin },
	{ "GetOrigin", ObjectOrbitMotivator_GetOrigin },
	{ "SetOrbit", ObjectOrbitMotivator_SetOrbit },
	{ "GetOrbit", ObjectOrbitMotivator_GetOrbit },
	{ "SetAngleASecond", ObjectOrbitMotivator_SetAngleASecond },
	{ "GetDegreesASecond", ObjectOrbitMotivator_GetDegreesASecond },
	{ "GetRadiansASecond", ObjectOrbitMotivator_GetRadiansASecond },
	{ "IsEnabled", ObjectOrbitMotivator_IsEnabled } ,
	{ "SetEnabled", ObjectOrbitMotivator_SetEnabled },
	{ nullptr, nullptr }
};

int ObjectOrbitMotivator_Constructor( lua_State * state )
{
	auto game = ScriptEngine::GetGame();

	int top = lua_gettop( state );
	int type = lua_type( state, 1 );

	ITexture::ptr texture;

	unify::V3< float > origin = CheckV3( state, 1 );
	unify::V3< float > orbit = CheckV3( state, 2 );
	unify::Angle angleASecond = unify::AngleInRadians( (float)luaL_checknumber( state, 3 ) );

	IObjectComponent::ptr component( new ObjectOrbitMotivator( origin, orbit, angleASecond ) );
	return PushObjectOrbitMotivator( state, component );
}

int ObjectOrbitMotivator_Destructor( lua_State * state )
{
	ObjectOrbitMotivatorProxy * proxy = CheckObjectOrbitMotivator( state, 1 );
	delete proxy;
	return 0;
}

void RegisterObjectOrbitMotivator( lua_State * state )
{
	lua_register( state, "ObjectOrbitMotivator", ObjectOrbitMotivator_Constructor );
	luaL_newmetatable( state, "ObjectOrbitMotivator" );
	lua_pushcfunction( state, ObjectOrbitMotivator_Destructor ); lua_setfield( state, -2, "__gc" );
	lua_pushvalue( state, -1 ); lua_setfield( state, -2, "__index" );
	luaL_setfuncs( state, ObjectOrbitMotivatorFunctions, 0 );
	lua_pop( state, 1 );
}

