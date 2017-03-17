// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <unify/ColorUnit.h>
															
MELUADLL_API int PushColor( lua_State * state, unify::ColorUnit color );
MELUADLL_API unify::ColorUnit CheckColor( lua_State * state, int index );

void RegisterColor( lua_State * state );
