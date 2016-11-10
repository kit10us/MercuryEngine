// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXILuaDLL.h>
#include <lua.hpp>
#include <me/Object.h>

struct ObjectProxy
{
	me::Object * object;
};

DXILUADLL_API int PushObject( lua_State * state, me::Object * object );
DXILUADLL_API ObjectProxy* CheckObject( lua_State* state, int index );

void RegisterObject( lua_State * state );
