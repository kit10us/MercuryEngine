// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <PxPhysicsAPI.h>
#include <memory>

std::shared_ptr< physx::PxPhysics > GetPhysX();

void RegisterPhysX( dxilua::ScriptEngine * luaSE );
