// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <me/Terra.h>

struct TerraProxy
{
	me::Geometry::ptr geometry;
	me::Terra * terra;
};

MELUADLL_API TerraProxy* CheckTerra( lua_State* state, int index );
MELUADLL_API int PushTerra( lua_State * state, me::Geometry::ptr geometry );

void RegisterTerra( lua_State * state );

