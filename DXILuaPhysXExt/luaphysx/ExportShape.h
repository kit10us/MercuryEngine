// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <PxPhysicsAPI.h>

struct PxShapeProxy
{
	std::shared_ptr< physx::PxShape > shape;
};

PxShapeProxy* CheckPxShape( lua_State* state, int index );
int PushPxShape( lua_State * state, std::shared_ptr< physx::PxShape > shape );

void RegisterPxShape( dxilua::ScriptEngine * luaSE );
