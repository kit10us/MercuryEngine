// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <unify/V2.h>

MELUADLL_API unify::V2< float > CheckV2( lua_State * state, int index );
MELUADLL_API int PushV2( lua_State * state, unify::V2< float > v2 );

int ExportV2( lua_State * state );
