// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/exports/ExportInputCondition.h>
#include <melua/exports/ExportInput.h>
#include <melua/ScriptEngine.h>
#include <melua/unify/ExportV3.h>

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
	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();

	std::string type = luaL_checkstring( state, 1 );

	me::input::IInputCondition::ptr inputCondition;

	if ( unify::StringIs( type, "button" ) )
	{
		size_t subSource = (size_t)luaL_checkinteger( state, 2 );
		std::string name = luaL_checkstring( state, 3 );
		std::string condition = luaL_checkstring( state, 4 );

		if ( unify::StringIs( condition, "down" ) )
		{
			inputCondition = me::input::IInputCondition::ptr( new me::input::ButtonCondition( subSource, name, true ) );
		}
		else if ( unify::StringIs( condition, "up" ) )
		{
			inputCondition = me::input::IInputCondition::ptr( new me::input::ButtonCondition( subSource, name, false ) );
		}
		else if ( unify::StringIs( condition, "pressed" ) )
		{
			inputCondition = me::input::IInputCondition::ptr( new me::input::ButtonPressedCondition( subSource, name ) );
		}
	}
	else if ( unify::StringIs( type, "stick" ) )
	{
		size_t subSource = (size_t)luaL_checkinteger( state, 2 );
		std::string name = luaL_checkstring( state, 3 );

		unify::V3< float > low_cap = CheckV3( state, 4 )->v3;
		unify::V3< float > low_threshold = CheckV3( state, 5 )->v3;
		unify::V3< float > high_threshold = CheckV3( state, 6 )->v3;
		unify::V3< float > high_cap = CheckV3( state, 7 )->v3;

		unify::V3< unify::Range< float > > low(
			unify::Range< float >( low_cap.x, low_threshold.x ),
			unify::Range< float >( low_cap.y, low_threshold.y ),
			unify::Range< float >( low_cap.z, low_threshold.z )
		);
		unify::V3< unify::Range< float > > high(
			unify::Range< float >( high_threshold.x, high_cap.x ),
			unify::Range< float >( high_threshold.y, high_cap.y ),
			unify::Range< float >( high_threshold.z, high_cap.z )
		);

		inputCondition = me::input::IInputCondition::ptr( new me::input::StickCondition( subSource, name, low, high ) );
	}
	else if ( unify::StringIs( type, "trigger" ) )
	{
		size_t subSource = (size_t)luaL_checkinteger( state, 2 );
		std::string name = luaL_checkstring( state, 3 );
		float threshold = (float)luaL_checknumber( state, 4 );
		float cap = (float)luaL_checknumber( state, 5 );
		inputCondition = me::input::IInputCondition::ptr( new me::input::TriggerCondition( subSource, name, threshold, cap ) );
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
	Type type = { "InputCondition", InputConditionFunctions, InputCondition_Constructor, InputCondition_Destructor };
	se->AddType( type );
}

