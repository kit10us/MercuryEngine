// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/motivator/AutoOIMotivator.h>

void RegisterObjectInputMotivator( lua_State * state );

struct ObjectInputMotivatorProxy
{
	me::object::ObjectComponent::ptr component;
	me::motivator::ObjectInputMotivator * motivator;
};

ObjectInputMotivatorProxy* CheckObjectInputMotivator( lua_State* state, int index );
int PushObjectInputMotivator( lua_State * state, me::object::ObjectComponent::ptr component );
