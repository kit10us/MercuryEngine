// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <unify/Matrix.h>

struct MatrixProxy
{
	unify::Matrix matrix;
};

MELUADLL_API int PushMatrix(lua_State * state, unify::Matrix martix );
MELUADLL_API MatrixProxy* CheckMatrix(lua_State * state, int index);

void RegisterMatrix(lua_State * state);