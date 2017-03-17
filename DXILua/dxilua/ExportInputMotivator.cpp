// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ExportInputMotivator.h>
#include <dxilua/ScriptEngine.h>
#include <dxilua/ExportInputCondition.h>
#include <dxilua/ExportObjectComponent.h>


#include <me/motivator/ObjectInputMotivator.h>

using namespace dxilua;
using namespace me;
using namespace motivator;

InputMotivatorProxy* CheckInputMotivator( lua_State* state, int index )
{
	InputMotivatorProxy* ud = *(InputMotivatorProxy**)luaL_checkudata( state, index, "InputMotivator" );
	return ud;
}

int PushInputMotivator( lua_State * state, me::motivator::IInputMotivator* motivator )
{
	InputMotivatorProxy ** proxy = (InputMotivatorProxy**)(lua_newuserdata( state, sizeof( InputMotivatorProxy* ) ));
	*proxy = new InputMotivatorProxy;
	luaL_setmetatable( state, "InputMotivator" );
	(*proxy)->motivator = motivator;
	return 1;
}														

int InputMotivator_Add( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	InputMotivatorProxy * motivatorProxy = CheckInputMotivator( state, 1 );
	std::string motivation = luaL_checkstring( state, 2 );
	InputConditionProxy * conditionProxy = CheckInputCondition( state, 3 );

	motivatorProxy->motivator->AddMotivation( motivation, conditionProxy->condition );

	return 0;
}

int InputMotivator_GetMotivationCount( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	InputMotivatorProxy * motivatorProxy = CheckInputMotivator( state, 1 );
	lua_pushnumber( state, motivatorProxy->motivator->GetMotivationCount() );

	return 1;
}

int InputMotivator_GetMotivationName( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	InputMotivatorProxy * motivatorProxy = CheckInputMotivator( state, 1 );
	int index = (int)lua_tonumber( state, 2 );

	lua_pushstring( state, motivatorProxy->motivator->GetMotivationName( index ).c_str() );

	return 0;
}

int InputMotivator_GetCondition( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	InputMotivatorProxy * motivatorProxy = CheckInputMotivator( state, 1 );
	auto type = lua_type( state, 2 );

	if ( type == LUA_TNUMBER )
	{
		int index = (int)lua_tonumber( state, 2 );
		auto condition = motivatorProxy->motivator->GetCondition( index );
		PushInputCondition( state, condition );
		return 1;
	}
	else if ( type == LUA_TSTRING )
	{
		std::string name = lua_tostring( state, 2 );
		auto condition = motivatorProxy->motivator->GetCondition( name );
		PushInputCondition( state, condition );
		return 1;
	}	
	else
	{
		lua_pushnil( state );
		return 1;
	}
}

int InputMotivator_Constructor( lua_State * state )
{
	auto game = dynamic_cast< me::IGame * >( ScriptEngine::GetGame() );
	int top = lua_gettop( state );

	if ( top < 1 )
	{
		game->ReportError( me::ErrorLevel::Failure, "LUA", "InputMotivator requires a component parameter!" );
	}

	int type = lua_type( state, 1 );

	ObjectComponentProxy* objectComponentProxy = CheckObjectComponent( state, 1 );		
	
	InputMotivatorProxy ** inputMotivatorProxy = (InputMotivatorProxy**)(lua_newuserdata( state, sizeof( InputMotivatorProxy* ) ));

	auto component = objectComponentProxy->component.get();

	{
		motivator::ObjectInputMotivator * oim = dynamic_cast< me::motivator::ObjectInputMotivator* >( component );
		*inputMotivatorProxy = new InputMotivatorProxy{ oim->GetMotivator() };
	}

	luaL_setmetatable( state, "InputMotivator" );

	return 1;
}

int InputMotivator_Destructor( lua_State * state )
{
	InputMotivatorProxy * proxy = CheckInputMotivator( state, 1 );
	delete proxy;
	return 0;
}

void RegisterInputMotivator( lua_State * state )
{
	const luaL_Reg memberFunctions[] =
	{
		{ "Add", InputMotivator_Add },
		{ "GetCount", InputMotivator_GetMotivationCount },
		{ "GetName", InputMotivator_GetMotivationName },
		{ "GetCondition", InputMotivator_GetCondition },
		{ nullptr, nullptr }
	};

	ScriptEngine * se = ScriptEngine::GetInstance();
	se->AddType( "InputMotivator", memberFunctions, sizeof( memberFunctions ) / sizeof( luaL_Reg ), InputMotivator_Constructor, InputMotivator_Destructor );
}

