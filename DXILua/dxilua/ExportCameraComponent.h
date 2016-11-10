// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/scene/CameraComponent.h>

void RegisterCameraComponent( lua_State * state );


struct CameraComponentProxy
{
	me::scene::ObjectComponent::ptr component;
	me::scene::CameraComponent * camera;
};

CameraComponentProxy* CheckCameraComponent( lua_State* state, int index );
int PushCameraComponent( lua_State * state, me::scene::ObjectComponent::ptr component );
