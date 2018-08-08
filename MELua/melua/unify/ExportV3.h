// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <unify/V3.h>

struct V3Proxy
{
	unify::V3< float > v3;
};

MELUADLL_API int PushV3( lua_State * state, unify::V3< float > v3 );
MELUADLL_API V3Proxy* CheckV3( lua_State * state, int index );

void RegisterV3( lua_State * state );