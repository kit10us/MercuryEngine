// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/object/CameraComponent.h>

void RegisterCameraComponent( lua_State * state );


struct CameraComponentProxy
{
	me::object::ObjectComponent::ptr component;
	me::object::CameraComponent * camera;
};

CameraComponentProxy* CheckCameraComponent( lua_State* state, int index );
int PushCameraComponent( lua_State * state, me::object::ObjectComponent::ptr component );
