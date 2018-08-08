// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <unify/Quaternion.h>

struct QuaternionProxy
{
	unify::Quaternion q;
};

MELUADLL_API int PushQuaternion(lua_State * state, unify::Quaternion martix);
MELUADLL_API QuaternionProxy* CheckQuaternion(lua_State * state, int index);

void RegisterQuaternion(lua_State * state);