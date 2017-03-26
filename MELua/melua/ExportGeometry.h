// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/Geometry.h>

struct GeometryProxy
{
	MELUADLL_API static char* Name();
	me::Geometry::ptr geometry;
};

void RegisterGeometry( lua_State * state );

