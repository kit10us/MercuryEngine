// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <unify/ColorUnit.h>

int PushColor( lua_State * state, unify::ColorUnit color );
unify::ColorUnit CheckColor( lua_State * state, int index );

