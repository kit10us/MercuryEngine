// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <unify/Size3.h>

MELUADLL_API unify::Size3< float > CheckSize3( lua_State * state, int index );
MELUADLL_API int PushSize3( lua_State * state, unify::Size3< float > v3 );

void RegisterSize3( lua_State * state );