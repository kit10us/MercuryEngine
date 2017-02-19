// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/motivator/ObjectOrbitMotivator.h>

void RegisterObjectOrbitMotivator( lua_State * state );

struct ObjectOrbitMotivatorProxy
{
	me::scene::ObjectComponent::ptr component;
	me::motivator::ObjectOrbitMotivator * motivator;
};

ObjectOrbitMotivatorProxy* CheckObjectOrbitMotivator( lua_State* state, int index );
int PushObjectOrbitMotivator( lua_State * state, me::scene::ObjectComponent::ptr component );
