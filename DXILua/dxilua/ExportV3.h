// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>

int ExportV3( lua_State * state );

unify::V3< float > lua_ToV3( lua_State * state, int index );
int lua_PushV3( lua_State * state, unify::V3< float > v3 );
