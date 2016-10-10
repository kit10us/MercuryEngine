// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>

int ExportV2( lua_State * state );

unify::V2< float > lua_ToV2( lua_State * state, int index );
int lua_PushV2( lua_State * state, unify::V3< float > v3 );
