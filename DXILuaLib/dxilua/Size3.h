// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <unify/Size3.h>

unify::Size3< float > CheckSize3( lua_State * state, int index );
int PushSize3( lua_State * state, unify::Size3< float > v3 );

