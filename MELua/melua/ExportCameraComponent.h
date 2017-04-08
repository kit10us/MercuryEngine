// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <me/object/CameraComponent.h>

void RegisterCameraComponent( lua_State * state );


struct CameraComponentProxy
{
	me::object::ObjectComponent::ptr component;
	me::object::CameraComponent * camera;
};

MELUADLL_API CameraComponentProxy* CheckCameraComponent( lua_State* state, int index );
MELUADLL_API int PushCameraComponent( lua_State * state, me::object::ObjectComponent::ptr component );
