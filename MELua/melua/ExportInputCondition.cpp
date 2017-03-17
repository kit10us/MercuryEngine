// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <melua/ExportInputCondition.h>
#include <melua/ExportInput.h>
#include <melua/ScriptEngine.h>

#include <me/input/ButtonCondition.h>
#include <me/input/ButtonPressedCondition.h>
#include <me/input/StickCondition.h>
#include <me/input/TriggerCondition.h>

using namespace melua;
using namespace me;
using namespace scene;

InputConditionProxy* CheckInputCondition( lua_State* state, int index )
{
	InputConditionProxy* ud = *(InputConditionProxy**)luaL_checkudata( state, index, "InputCondition" );
	return ud;
}

int PushInputCondition( lua_State * state, me::input::IInputCondition::ptr condition )
{
	InputConditionProxy ** proxy = (InputConditionProxy**)(lua_newuserdata( state, sizeof( InputConditionProxy* ) ));
	*proxy = new InputConditionProxy;
	luaL_setmetatable( state, "InputCondition" );
	(*proxy)->condition = condition;
	return 1;
}													

static const luaL_Reg InputConditionFunctions[] =
{
	{ nullptr, nullptr }
};

int InputCondition_Constructor( lua_State * state )
{
	auto game = ScriptEngine::GetGame();

	std::string type = luaL_checkstring( state, 1 );
	me::input::IInputCondition::ptr inputCondition;

	if ( unify::StringIs( type, "button" ) )
	{
		InputProxy * source = CheckInput( state, 2 );
		size_t subSource = (size_t)luaL_checkinteger( state, 3 );
		std::string name = luaL_checkstring( state, 4 );
		std::string condition = luaL_checkstring( state, 5 );
		if ( unify::StringIs( condition, "down" ) )
		{
			inputCondition = me::input::IInputCondition::ptr( new me::input::ButtonCondition( source->input, subSource, name, true ) );
		}
		else if ( unify::StringIs( condition, "up" ) )
		{
			inputCondition = me::input::IInputCondition::ptr( new me::input::ButtonCondition( source->input, subSource, name, false ) );
		}
		else if ( unify::StringIs( condition, "pressed" ) )
		{
			inputCondition = me::input::IInputCondition::ptr( new me::input::ButtonPressedCondition( source->input, subSource, name ) );
		}
	}
	else if ( unify::StringIs( type, "stick" ) )
	{
		InputProxy * source = CheckInput( state, 2 );
		size_t subSource = (size_t)luaL_checkinteger( state, 3 );
		std::string name = luaL_checkstring( state, 4 );
		std::string axisString = luaL_checkstring( state, 5 );
		input::StickAxis axis = input::StickAxisFromString( axisString );
		float cap_low = (float)luaL_checknumber( state, 6 );
		float threshold_low = (float)luaL_checknumber( state, 7 );
		float threshold_high = (float)luaL_checknumber( state, 8 );
		float cap_high = (float)luaL_checknumber( state, 9 );
		inputCondition = me::input::IInputCondition::ptr( new me::input::StickCondition( source->input, subSource, name, axis, cap_low, threshold_low, threshold_high, cap_high ) );
	}
	else if ( unify::StringIs( type, "trigger" ) )
	{
		InputProxy * source = CheckInput( state, 2 );
		size_t subSource = (size_t)luaL_checkinteger( state, 3 );
		std::string name = luaL_checkstring( state, 4 );
		float threshold = (float)luaL_checknumber( state, 5 );
		float cap = (float)luaL_checknumber( state, 6 );
		inputCondition = me::input::IInputCondition::ptr( new me::input::TriggerCondition( source->input, subSource, name, threshold, cap ) );
	}

	return PushInputCondition( state, inputCondition );
}

int InputCondition_Destructor( lua_State * state )
{
	InputConditionProxy * proxy = CheckInputCondition( state, 1 );
	delete proxy;
	return 0;
}

void RegisterInputCondition( lua_State * state )
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	se->AddType( "InputCondition", InputConditionFunctions, sizeof( InputConditionFunctions ) / sizeof( luaL_Reg ), InputCondition_Constructor, InputCondition_Destructor );
}

