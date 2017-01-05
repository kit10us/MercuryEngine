// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <PxPhysicsAPI.h>
#include <dxiphysx/SceneComponent.h>

struct PxSceneComponentProxy
{
	me::scene::ISceneComponent::ptr sceneComponent;
	dxiphysx::SceneComponent * scene;
};

PxSceneComponentProxy* CheckPxSceneComponent( lua_State* state, int index );
int PushPxSceneComponent( lua_State * state, dxiphysx::SceneComponent::ptr sceneComponent );

void RegisterPxSceneComponent( dxilua::ScriptEngine * luaSE, me::IGame * game );
