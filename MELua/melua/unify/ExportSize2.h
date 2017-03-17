// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <unify/Size.h>

MELUADLL_API unify::Size< float > CheckSize2( lua_State * state, int index );
MELUADLL_API int PushSize2( lua_State * state, unify::Size< float > size8 );

void RegisterSize2( lua_State * state );