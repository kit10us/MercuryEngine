// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <unify/TexArea.h>

MELUADLL_API unify::TexArea CheckTexArea( lua_State * state, int index );
MELUADLL_API int PushTexArea( lua_State * state, unify::TexArea texArea );

int ExportTexArea( lua_State * state );
