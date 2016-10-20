// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXILuaDLL.h>
#include <lua.hpp>
#include <unify/V3.h>

DXILUADLL_API unify::V3< float > CheckV3( lua_State * state, int index );
DXILUADLL_API int PushV3( lua_State * state, unify::V3< float > v3 );

void RegisterV3( lua_State * state );