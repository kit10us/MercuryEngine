// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXILuaDLL.h>
#include <lua.hpp>
#include <unify/TexCoords.h>

DXILUADLL_API unify::TexCoords CheckTexCoords( lua_State * state, int index );
DXILUADLL_API int PushTexCoords( lua_State * state, unify::TexCoords texCoords );

int ExportTexCoords( lua_State * state );
