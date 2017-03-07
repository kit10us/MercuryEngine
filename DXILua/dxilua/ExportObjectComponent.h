// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/scene/ObjectComponent.h>

void RegisterObjectComponent( lua_State * state );


struct ObjectComponentProxy
{
	me::scene::ObjectComponent::ptr component;
};

ObjectComponentProxy* CheckObjectComponent( lua_State* state, int index );
int PushObjectComponent( lua_State * state, me::scene::ObjectComponent::ptr component );
