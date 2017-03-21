// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <unify/TexCoords.h>

struct TexCoordsProxy
{
	unify::TexCoords texcoords;
};

MELUADLL_API int PushTexCoords(lua_State * state, unify::TexCoords texcoords );
MELUADLL_API TexCoordsProxy* CheckTexCoords(lua_State * state, int index);

void RegisterTexCoords(lua_State * state);