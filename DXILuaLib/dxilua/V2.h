// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <unify/V2.h>

unify::V2< float > CheckV2( lua_State * state, int index );
int PushV2( lua_State * state, unify::V2< float > v2 );
