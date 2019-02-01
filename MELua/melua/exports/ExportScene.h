// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/scene/IScene.h>

void RegisterScene( lua_State * state );

struct SceneProxy
{
	me::scene::IScene* scene;
};

int PushScene( lua_State * state, me::scene::IScene * scene );
MELUADLL_API SceneProxy* CheckScene( lua_State* state, int index );

void RegisterSceneFunctions( lua_State * state );
