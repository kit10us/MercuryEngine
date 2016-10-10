// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <dxi/scene/Camera.h>

void RegisterCameraComponent( lua_State * state );

struct CameraComponentProxy
{
	dxi::scene::Camera * camera;
};

CameraComponentProxy* CheckCameraComponent( lua_State* state, int index );
