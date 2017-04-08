// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <me/render/Effect.h>

struct EffectProxy
{
	MELUADLL_API static char* Name();

	me::render::Effect::ptr effect;
};

void RegisterEffect( lua_State * state );

