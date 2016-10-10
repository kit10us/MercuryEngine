// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <dxi/scene/Scene.h>

void RegisterScene( lua_State * state );

struct SceneProxy
{
	dxi::scene::Scene::shared_ptr scene;
};

SceneProxy* CheckScene( lua_State* state, int index );