// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <unify/Matrix.h>

MELUADLL_API unify::Matrix CheckMatrix( lua_State* state, int index );
MELUADLL_API int PushMatrix( lua_State* state, const unify::Matrix & mat );

int ExportMatrix( lua_State * state );
