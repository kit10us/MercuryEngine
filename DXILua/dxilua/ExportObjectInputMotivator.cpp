// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ExportObjectInputMotivator.h>
#include <dxilua/ExportObject.h>
#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/ScriptEngine.h>

#include <dxilua/ExportInputCondition.h>

using namespace dxilua;
using namespace me;
using namespace scene;
using namespace object;
using namespace motivator;

ObjectInputMotivatorProxy* CheckObjectInputMotivator( lua_State* state, int index )
{
	ObjectInputMotivatorProxy* ud = *(ObjectInputMotivatorProxy**)luaL_checkudata( state, index, "ObjectInputMotivator" );
	return ud;
}

int PushObjectInputMotivator( lua_State * state, me::object::ObjectComponent::ptr component )
{
	ObjectInputMotivatorProxy ** proxy = (ObjectInputMotivatorProxy**)(lua_newuserdata( state, sizeof( ObjectInputMotivatorProxy* ) ));
	*proxy = new ObjectInputMotivatorProxy;
	luaL_setmetatable( state, "ObjectInputMotivator" );
	(*proxy)->component = component;
	(*proxy)->motivator = dynamic_cast< ObjectInputMotivator *>(component.get());
	return 1;
}														

int ObjectInputMotivator_AttachTo( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectInputMotivatorProxy * cameraComponentProxy = CheckObjectInputMotivator( state, 1 );
	ObjectProxy * objectProxy = CheckObject( state, 2 );
	objectProxy->object->AddComponent( cameraComponentProxy->component );
	return 0;
}

int ObjectInputMotivator_Add( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	ObjectInputMotivatorProxy * motivatorProxy = CheckObjectInputMotivator( state, 1 );
	std::string motivation = luaL_checkstring( state, 2 );
	InputConditionProxy * conditionProxy = CheckInputCondition( state, 3 );

	motivatorProxy->motivator->GetMotivator()->AddMotivation( motivation, conditionProxy->condition );

	return 0;
}

int ObjectInputMotivator_SetEnabled( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectInputMotivatorProxy * motivatorProxy = CheckObjectInputMotivator( state, 1 );
	bool enabled = lua_toboolean( state, 2 ) ? true : false;

	motivatorProxy->motivator->SetEnabled( enabled );
	return 0;
}

int ObjectInputMotivator_IsEnabled( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectInputMotivatorProxy * motivatorProxy = CheckObjectInputMotivator( state, 1 );

	lua_pushboolean( state, motivatorProxy->motivator->IsEnabled() );

	return 1;
}

int ObjectInputMotivator_SetValue( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	ObjectInputMotivatorProxy * motivatorProxy = CheckObjectInputMotivator( state, 1 );
	std::string name = luaL_checkstring( state, 2 );

	int type = lua_type( state, 3 );
	switch ( type )
	{
	case LUA_TNIL:
		assert( 0 );
		break;
	case LUA_TBOOLEAN:
		motivatorProxy->motivator->SetValue( name, unify::Cast< std::string >( lua_toboolean( state, 3 ) ) );
		break;
	case LUA_TNUMBER:
		motivatorProxy->motivator->SetValue( name, unify::Cast< std::string >( luaL_checknumber( state, 3 ) ) );
		break;
	case LUA_TSTRING:
		motivatorProxy->motivator->SetValue( name, luaL_checkstring( state, 3 ) );
		break;
	}
	bool enabled = lua_toboolean( state, 2 ) ? true : false;

	motivatorProxy->motivator->SetEnabled( enabled );
	return 0;
}

static const luaL_Reg ObjectInputMotivatorFunctions[] =
{
	{ "AttachTo", ObjectInputMotivator_AttachTo },
	{ "Add", ObjectInputMotivator_Add },
	{ "IsEnabled", ObjectInputMotivator_IsEnabled },
	{ "SetEnabled", ObjectInputMotivator_SetEnabled },
	{ "SetValue", ObjectInputMotivator_SetValue },
	{ nullptr, nullptr }
};

int ObjectInputMotivator_Constructor( lua_State * state )
{
	auto game = ScriptEngine::GetGame();

	int top = lua_gettop( state );
	int type = lua_type( state, 1 );

	IObjectComponent::ptr component( new motivator::AutoOIMotivator() );
	return PushObjectInputMotivator( state, component );
}

int ObjectInputMotivator_Destructor( lua_State * state )
{
	ObjectInputMotivatorProxy * proxy = CheckObjectInputMotivator( state, 1 );
	delete proxy;
	return 0;
}

void RegisterObjectInputMotivator( lua_State * state )
{
	lua_register( state, "ObjectInputMotivator", ObjectInputMotivator_Constructor );
	luaL_newmetatable( state, "ObjectInputMotivator" );
	lua_pushcfunction( state, ObjectInputMotivator_Destructor ); lua_setfield( state, -2, "__gc" );
	lua_pushvalue( state, -1 ); lua_setfield( state, -2, "__index" );
	luaL_setfuncs( state, ObjectInputMotivatorFunctions, 0 );
	lua_pop( state, 1 );
}

