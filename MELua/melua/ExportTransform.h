// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/object/Object.h>

void RegisterTransform( lua_State * state );

struct TransformProxy
{
	me::object::Object * object;
	unify::FrameLite * transform;
};

TransformProxy* CheckTransform( lua_State* state, int index );

