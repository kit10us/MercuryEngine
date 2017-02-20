// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/motivator/ObjectInputMotivator.h>

void RegisterObjectInputMotivator( lua_State * state );

struct ObjectInputMotivatorProxy
{
	me::scene::ObjectComponent::ptr component;
	me::motivator::ObjectInputMotivator * motivator;
};

ObjectInputMotivatorProxy* CheckObjectInputMotivator( lua_State* state, int index );
int PushObjectInputMotivator( lua_State * state, me::scene::ObjectComponent::ptr component );
