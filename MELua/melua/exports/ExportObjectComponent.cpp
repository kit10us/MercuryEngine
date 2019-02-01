// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <melua/exports/ExportObjectComponent.h>
#include <melua/exports/ExportObject.h>
#include <melua/exports/ExportComponent.h>
#include <melua/unify/ExportMatrix.h>
#include <melua/ScriptEngine.h>

#include <melua/unify/ExportMatrix.h>
#include <melua/unify/ExportColor.h>
#include <melua/unify/ExportSize2.h>
#include <melua/unify/ExportSize3.h>
#include <melua/unify/ExportV2.h>
#include <melua/unify/ExportV3.h>

using namespace me;
using namespace scene;


using namespace melua;

char* ObjectComponentProxy::Name()
{
	return "ObjectComponent";
}

/*

ObjectComponentProxy* CheckUserType< ObjectComponentProxy >( lua_State* state, int index )
{
	ObjectComponentProxy* ud = *(ObjectComponentProxy**)luaL_checkudata( state, index, "ObjectComponent" );
	return ud;
}

int PushObjectComponent( lua_State * state, me::object::component::ObjectComponent::ptr component )
{
	ObjectComponentProxy ** proxy = (ObjectComponentProxy**)(lua_newuserdata( state, sizeof( ObjectComponentProxy* ) ));
	*proxy = new ObjectComponentProxy;
	luaL_setmetatable( state, "ObjectComponent" );
	(*proxy)->component = component;
	return 1;
}				
*/

int ObjectComponent_SetEnabled( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectComponentProxy * componentProxy = CheckUserType< ObjectComponentProxy >( state, 1 );
	bool enabled = lua_toboolean( state, 2 ) ? true : false;

	componentProxy->component->SetEnabled( enabled );
	return 0;
}

int ObjectComponent_GetEnabled( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectComponentProxy * componentProxy = CheckUserType< ObjectComponentProxy >( state, 1 );

	lua_pushboolean( state, componentProxy->component->IsEnabled() );

	return 1;
}					

int ObjectComponent_GetType( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectComponentProxy * componentProxy = CheckUserType< ObjectComponentProxy >( state, 1 );
	lua_pushstring( state, componentProxy->component->GetTypeName().c_str() );
	return 1;
}			

int ObjectComponent_GetWhat( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectComponentProxy * componentProxy = CheckUserType< ObjectComponentProxy >( state, 1 );
	lua_pushstring( state, componentProxy->component->GetWhat().c_str() );
	return 1;
}			

int ObjectComponent_GetValueCount( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	ObjectComponentProxy * componentProxy = CheckUserType< ObjectComponentProxy >( state, 1 );
	lua_pushnumber( state, componentProxy->component->GetLookup()->Count() );
	return 1;
}			

int ObjectComponent_ValueExists( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectComponentProxy * componentProxy = CheckUserType< ObjectComponentProxy >( state, 1 );

	std::string name = lua_tostring( state, 2 );

	lua_pushboolean( state, componentProxy->component->GetLookup()->Exists( name ) );
	return 1;
}			

int ObjectComponent_GetValueName( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectComponentProxy * componentProxy = CheckUserType< ObjectComponentProxy >( state, 1 );

	int index = (int)lua_tonumber( state, 2 );

	lua_pushstring( state, componentProxy->component->GetLookup()->GetName( index ).c_str() );
	return 1;
}			

int ObjectComponent_FindValueIndex( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectComponentProxy * componentProxy = CheckUserType< ObjectComponentProxy >( state, 1 );

	std::string name = lua_tostring( state, 2 );

	lua_pushnumber( state, componentProxy->component->GetLookup()->Find( name ) );
	return 1;
}			

int ObjectComponent_GetValue( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ObjectComponentProxy * componentProxy = CheckUserType< ObjectComponentProxy >( state, 1 );

	int type = lua_type( state, 2 );

	if ( type == LUA_TNUMBER )
	{
		int index = (int)lua_tonumber( state, 2 );
		lua_pushstring( state, componentProxy->component->GetLookup()->GetValue( index ).c_str() );
		return 1;
	}
	else if ( type == LUA_TSTRING )
	{
		std::string name = lua_tostring( state, 2 );
		lua_pushstring( state, componentProxy->component->GetLookup()->GetValue( name ).c_str() );
		return 1;
	}
	else
	{
		return 0;
	}
}

int ObjectComponent_SetValue( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	ObjectComponentProxy * componentProxy = CheckUserType< ObjectComponentProxy >( state, 1 );

	int type = lua_type( state, 2 );
	
	int valueType = lua_type( state, 3 );
	std::string value;
	if ( valueType == LUA_TSTRING )
	{
		value = lua_tostring( state, 3 );
	}
	else if ( valueType == LUA_TNUMBER )
	{
		value = unify::Cast< std::string >( lua_tonumber( state, 3 ) );
	}
	else if ( valueType == LUA_TBOOLEAN )
	{
		value = unify::Cast< std::string >( lua_toboolean( state, 3 ) );
	}
	else
	{
		value = "";
	}

	if ( type == LUA_TNUMBER )
	{
		int index = (int)lua_tonumber( state, 2 );
		state, componentProxy->component->GetLookup()->SetValue( index, value );
		return 0;
	}
	else if ( type == LUA_TSTRING )
	{
		std::string name = lua_tostring( state, 2 );
		componentProxy->component->GetLookup()->SetValue( name, value );
		return 0;
	}
	else
	{
		return 0;
	}
}

int ObjectComponent_Constructor( lua_State * state )
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();

	auto type = GetTypename( state );
	if( unify::string::StringIs( type, "ObjectComponent" ) )
	{
		return PushUserType< ObjectComponentProxy >( state, *CheckUserType< ObjectComponentProxy >( state, 1 ) );
	}
	else if( unify::string::StringIs( type, "Component" ) )
	{
		return PushUserType< ComponentProxy >( state, *CheckUserType< ComponentProxy >( state, 1 ) );
	}

	return PushNil( state );
}

int ObjectComponent_Destructor( lua_State * state )
{
	ObjectComponentProxy * proxy = CheckUserType< ObjectComponentProxy >( state, 1 );
	delete proxy;
	return 0;
}

void RegisterObjectComponent( lua_State * state )
{
	const luaL_Reg memberFunctions[] =
	{
		{ "SetEnabled", ObjectComponent_SetEnabled },
		{ "GetEnabled", ObjectComponent_GetEnabled },
		{ "GetType", ObjectComponent_GetType },
		{ "GetWhat", ObjectComponent_GetWhat },
		{ "GetValueCount", ObjectComponent_GetValueCount },
		{ "ValueExists", ObjectComponent_ValueExists },
		{ "GetValueName", ObjectComponent_GetValueName },
		{ "FindValueIndex", ObjectComponent_FindValueIndex },
		{ "GetValue", ObjectComponent_GetValue },
		{ "SetValue", ObjectComponent_SetValue }, 
		{ nullptr, nullptr }
	};			   

	ScriptEngine * se = ScriptEngine::GetInstance();
	Type type = { "ObjectComponent", memberFunctions, ObjectComponent_Constructor, ObjectComponent_Destructor };
	se->AddType( type );
}

