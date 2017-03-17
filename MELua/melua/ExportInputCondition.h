// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/input/IInputCondition.h>

void RegisterInputCondition( lua_State * state );
	  
struct InputConditionProxy
{
	me::input::IInputCondition::ptr condition;
};

InputConditionProxy* CheckInputCondition( lua_State* state, int index );
int PushInputCondition( lua_State * state, me::input::IInputCondition::ptr condition );
