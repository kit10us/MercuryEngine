// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXILuaDLL.h>
#include <lua.hpp>
#include <dxi/scene/Object.h>

struct ObjectProxy
{
	dxi::scene::Object * object;
};

DXILUADLL_API int PushObject( lua_State * state, dxi::scene::Object * object );
DXILUADLL_API ObjectProxy* CheckObject( lua_State* state, int index );

void RegisterObject( lua_State * state );
