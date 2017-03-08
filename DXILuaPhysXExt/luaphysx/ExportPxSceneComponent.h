// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <PxPhysicsAPI.h>
#include <mephysx/SceneComponent.h>

struct PxSceneComponentProxy
{
	me::scene::ISceneComponent::ptr sceneComponent;
	mephysx::SceneComponent * scene;
};

PxSceneComponentProxy* CheckPxSceneComponent( lua_State* state, int index );
int PushPxSceneComponent( lua_State * state, mephysx::SceneComponent::ptr sceneComponent );

void RegisterPxSceneComponent( dxilua::ScriptEngine * luaSE, me::IGame * game );
