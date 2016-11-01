// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXILuaDLL.h>
#include <lua.hpp>
#include <unify/Quaternion.h>

DXILUADLL_API unify::Quaternion CheckQuaternion( lua_State * state, int index );
DXILUADLL_API int PushQuaternion( lua_State * state, unify::Quaternion v3 );

void ExportQuaternion( lua_State * state );