// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <unify/TexCoords.h>

MELUADLL_API unify::TexCoords CheckTexCoords( lua_State * state, int index );
MELUADLL_API int PushTexCoords( lua_State * state, unify::TexCoords texCoords );

int ExportTexCoords( lua_State * state );
