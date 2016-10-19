// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <dxi/scene/CameraComponent.h>

void RegisterCameraComponent( lua_State * state );


struct CameraComponentProxy
{
	dxi::scene::ObjectComponent::ptr component;
	dxi::scene::CameraComponent * camera;
};

CameraComponentProxy* CheckCameraComponent( lua_State* state, int index );
int PushCameraComponent( lua_State * state, dxi::scene::ObjectComponent::ptr component );