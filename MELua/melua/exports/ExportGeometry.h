// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/render/Geometry.h>

struct GeometryProxy
{
	MELUADLL_API static char* Name();
	me::render::Geometry::ptr geometry;
};

void RegisterGeometry( lua_State * state );

