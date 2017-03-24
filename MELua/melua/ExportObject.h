// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <me/object/Object.h>

struct ObjectProxy
{
	me::object::Object * object;
};

MELUADLL_API int PushObject( lua_State * state, me::object::Object * object );
MELUADLL_API ObjectProxy* CheckObject( lua_State* state, int index );

void RegisterObject( lua_State * state );