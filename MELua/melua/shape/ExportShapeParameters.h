// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <unify/Parameters.h>

struct ShapeParameters
{
	unify::Parameters parameters;
};

int PushShapeParameters( lua_State * state, ShapeParameters & parameters );						  
ShapeParameters * CheckShapeParameters( lua_State* state, int index );

void RegisterShapeParameters( lua_State * state );
