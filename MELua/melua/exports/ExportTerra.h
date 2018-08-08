// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <me/render/Terra.h>

struct TerraProxy
{
	me::render::Geometry::ptr geometry;
	me::render::Terra * terra;
};

MELUADLL_API TerraProxy* CheckTerra( lua_State* state, int index );
MELUADLL_API int PushTerra( lua_State * state, me::render::Geometry::ptr geometry );

void RegisterTerra( lua_State * state );

