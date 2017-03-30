// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <me/Effect.h>

struct EffectProxy
{
	MELUADLL_API static char* Name();

	me::Effect::ptr effect;
};

void RegisterEffect( lua_State * state );

