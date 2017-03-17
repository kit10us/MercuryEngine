// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/Geometry.h>

struct GeometryProxy
{
	me::Geometry::ptr geometry;
};

GeometryProxy* CheckGeometry( lua_State* state, int index );
int PushGeometry( lua_State * state, me::Geometry::ptr geometry );

void RegisterGeometry( lua_State * state );

