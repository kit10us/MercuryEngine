// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <mephysx/objectcomponent/RigidStatic.h>

struct PxRigidStaticProxy
{
	mephysx::objectcomponent::RigidStatic::ptr component;
	physx::PxRigidStatic * body;
};

PxRigidStaticProxy* CheckPxRigidStatic( lua_State* state, int index );
int PushPxRigidStatic( lua_State * state, mephysx::objectcomponent::RigidStatic::ptr component );

void RegisterPxRigidStatic( dxilua::ScriptEngine * luaSE, me::IGame * game );
