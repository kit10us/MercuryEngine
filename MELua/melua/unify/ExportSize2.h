// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <unify/Size.h>

struct Size2Proxy
{
	unify::Size< float > size2;
};

MELUADLL_API int PushSize2(lua_State * state, unify::Size< float > size2);
MELUADLL_API Size2Proxy* CheckSize2(lua_State * state, int index);

void RegisterSize2(lua_State * state);