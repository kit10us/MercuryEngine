// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>

void RegisterObject( lua_State * state );

struct ObjectProxy
{
	dxi::scene::Object::shared_ptr object;
};

ObjectProxy* CheckObject( lua_State* state, int index );


