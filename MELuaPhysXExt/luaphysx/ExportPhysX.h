// Copyright (c) 2002 - 2011, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <PxPhysicsAPI.h>
#include <memory>

std::shared_ptr< physx::PxPhysics > GetPhysX();

void RegisterPhysX( melua::ScriptEngine * luaSE, me::IGame * game );
