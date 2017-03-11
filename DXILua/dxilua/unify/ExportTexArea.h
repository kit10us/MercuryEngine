// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXILuaDLL.h>
#include <lua.hpp>
#include <unify/TexArea.h>

DXILUADLL_API unify::TexArea CheckTexArea( lua_State * state, int index );
DXILUADLL_API int PushTexArea( lua_State * state, unify::TexArea texArea );

int ExportTexArea( lua_State * state );
