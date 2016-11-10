// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/CameraComponent.h>

void RegisterCameraComponent( lua_State * state );


struct CameraComponentProxy
{
	me::ObjectComponent::ptr component;
	me::CameraComponent * camera;
};

CameraComponentProxy* CheckCameraComponent( lua_State* state, int index );
int PushCameraComponent( lua_State * state, me::ObjectComponent::ptr component );