// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXILuaDLL.h>
#include <lua.hpp>
#include <unify/Matrix.h>

DXILUADLL_API unify::Matrix CheckMatrix( lua_State* state, int index );
DXILUADLL_API int PushMatrix( lua_State* state, const unify::Matrix & mat );

int ExportMatrix( lua_State * state );
