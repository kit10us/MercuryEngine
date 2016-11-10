// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXILuaDLL.h>
#include <lua.hpp>
#include <dxi/Terra.h>

struct TerraProxy
{
	me::Geometry::ptr geometry;
	dxi::Terra * terra;
};

DXILUADLL_API TerraProxy* CheckTerra( lua_State* state, int index );
DXILUADLL_API int PushTerra( lua_State * state, me::Geometry::ptr geometry );

void RegisterTerra( lua_State * state );

