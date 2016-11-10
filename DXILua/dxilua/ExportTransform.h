// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/scene/Object.h>

void RegisterTransform( lua_State * state );

struct TransformProxy
{
	me::scene::Object * object;
	unify::FrameLite * transform;
};

TransformProxy* CheckTransform( lua_State* state, int index );


