// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <unify/Matrix.h>

struct MatrixProxy
{
	MELUADLL_API static char* Name();
	unify::Matrix matrix;
};

void RegisterMatrix(lua_State * state);