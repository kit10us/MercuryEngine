// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <unify/V3.h>

unify::V3< float > CheckV3( lua_State * state, int index );
int PushV3( lua_State * state, unify::V3< float > v3 );

