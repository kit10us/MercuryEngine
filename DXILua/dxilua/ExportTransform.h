// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <dxi/scene/Object.h>

void RegisterTransform( lua_State * state );

struct TransformProxy
{
	dxi::scene::Object * object;
	unify::FrameLite * transform;
};

TransformProxy* CheckTransform( lua_State* state, int index );


