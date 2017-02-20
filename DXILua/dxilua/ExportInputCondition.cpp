// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ExportInputCondition.h>
#include <dxilua/ExportInput.h>
#include <dxilua/ScriptEngine.h>

using namespace dxilua;
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
		inputCondition = me::input::IInputCondition::ptr( me::input::MakeButtonCondition( source->input, subSource, name, condition ) );
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
	lua_register( state, "InputCondition", InputCondition_Constructor );
	luaL_newmetatable( state, "InputCondition" );
	lua_pushcfunction( state, InputCondition_Destructor ); lua_setfield( state, -2, "__gc" );
	lua_pushvalue( state, -1 ); lua_setfield( state, -2, "__index" );
	luaL_setfuncs( state, InputConditionFunctions, 0 );
	lua_pop( state, 1 );
}

