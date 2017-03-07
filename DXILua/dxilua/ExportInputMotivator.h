// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/motivator/IInputMotivator.h>

void RegisterInputMotivator( lua_State * state );

struct InputMotivatorProxy
{
	me::motivator::IInputMotivator * motivator;
};

InputMotivatorProxy* CheckInputMotivator( lua_State* state, int index );
int PushInputMotivator( lua_State * state, me::motivator::IInputMotivator * motivator );
