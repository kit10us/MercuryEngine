// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <PxPhysicsAPI.h>
#include <dxiphysx/SceneComponent.h>

struct PxSceneComponentProxy
{
	dxiphysx::SceneComponent::ptr sceneComponent;
};

PxSceneComponentProxy* CheckPxSceneComponent( lua_State* state, int index );
int PushPxSceneComponent( lua_State * state, dxiphysx::SceneComponent::ptr sceneComponent );

void RegisterPxSceneComponent( dxilua::ScriptEngine * luaSE, me::IGame * game );
