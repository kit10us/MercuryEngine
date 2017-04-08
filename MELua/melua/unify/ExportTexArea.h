// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <unify/TexArea.h>

struct TexAreaProxy
{
	unify::TexArea texarea;
};

MELUADLL_API int PushTexArea(lua_State * state, unify::TexArea texarea);
MELUADLL_API TexAreaProxy* CheckTexArea(lua_State * state, int index);

void RegisterTexArea(lua_State * state);