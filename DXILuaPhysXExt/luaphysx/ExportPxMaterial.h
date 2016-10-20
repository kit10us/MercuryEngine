// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <PxPhysicsAPI.h>
#include <memory>

struct PxMaterialProxy
{
	std::shared_ptr< physx::PxMaterial > material;
};

PxMaterialProxy* CheckPxMaterial( lua_State* state, int index );
int PushPxMaterial( lua_State * state, std::shared_ptr< physx::PxMaterial > shape );

void RegisterPxMaterial( dxilua::ScriptEngine * luaSE, dxi::core::Game * game );
