// Copyright (c) 2002 - 2011, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <mephysx/objectcomponent/RigidBody.h>

struct PxRigidBodyProxy
{
	me::object::IObjectComponent::ptr component;
	physx::PxRigidBody * body;
};

PxRigidBodyProxy* CheckPxRigidBody( lua_State* state, int index );
int PushPxRigidBody( lua_State * state, mephysx::objectcomponent::RigidBody::ptr component );

void RegisterPxRigidBody( melua::ScriptEngine * luaSE, me::IGame * game );
