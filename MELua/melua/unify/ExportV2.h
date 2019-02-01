// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <unify/V2.h>

struct V2Proxy
{
	unify::V2< float > v2;
};

MELUADLL_API int PushV2(lua_State * state, unify::V2< float > v2);
MELUADLL_API V2Proxy* CheckV2(lua_State * state, int index);

void RegisterV2(lua_State * state);