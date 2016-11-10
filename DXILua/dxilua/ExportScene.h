// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/scene/Scene.h>

void RegisterScene( lua_State * state );

struct SceneProxy
{
	me::scene::Scene::ptr scene;
};

DXILUADLL_API SceneProxy* CheckScene( lua_State* state, int index );