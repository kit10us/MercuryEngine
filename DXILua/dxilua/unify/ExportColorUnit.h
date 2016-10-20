// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXILuaDLL.h>
#include <lua.hpp>
#include <unify/ColorUnit.h>
															
DXILUADLL_API int PushColorUnit( lua_State * state, unify::ColorUnit color );
DXILUADLL_API unify::ColorUnit CheckColor( lua_State * state, int index );

void RegisterColor( lua_State * state );
