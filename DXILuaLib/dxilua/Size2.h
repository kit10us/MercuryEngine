// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <unify/Size.h>

unify::Size< float > CheckSize2( lua_State * state, int index );
int PushSize2( lua_State * state, unify::Size< float > v3 );
	