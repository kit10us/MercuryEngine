// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <dxi/Effect.h>

struct EffectProxy
{
	dxi::Effect::ptr effect;
};

EffectProxy* CheckEffect( lua_State* state, int index );
int PushEffect( lua_State * state, dxi::Effect::ptr effect );

void RegisterEffect( lua_State * state );

