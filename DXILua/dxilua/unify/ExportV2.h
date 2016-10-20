// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXILuaDLL.h>
#include <lua.hpp>
#include <unify/V2.h>

DXILUADLL_API unify::V2< float > CheckV2( lua_State * state, int index );
DXILUADLL_API int PushV2( lua_State * state, unify::V2< float > v2 );

int ExportV2( lua_State * state );
