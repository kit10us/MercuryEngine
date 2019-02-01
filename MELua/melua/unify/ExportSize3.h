// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <unify/Size3.h>

struct Size3Proxy
{
	unify::Size3< float > size3;
};

MELUADLL_API int PushSize3(lua_State * state, unify::Size3< float > size3);
MELUADLL_API Size3Proxy* CheckSize3(lua_State * state, int index);

void RegisterSize3(lua_State * state);