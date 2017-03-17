// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/IComponent.h>

void RegisterComponent( lua_State * state );


struct ComponentProxy
{
	me::IComponent* component;
};

ComponentProxy* CheckComponent( lua_State* state, int index );
int PushComponent( lua_State * state, me::IComponent* component );
