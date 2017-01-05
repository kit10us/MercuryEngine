// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <dxiphysx/objectcomponent/RigidBody.h>

struct PxRigidBodyProxy
{
	me::scene::IObjectComponent::ptr component;
	physx::PxRigidBody * body;
};

PxRigidBodyProxy* CheckPxRigidBody( lua_State* state, int index );
int PushPxRigidBody( lua_State * state, dxiphysx::objectcomponent::RigidBody::ptr component );

void RegisterPxRigidBody( dxilua::ScriptEngine * luaSE, me::IGame * game );
