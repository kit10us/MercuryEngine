// Copyright (c) 2002 - 2011, Evil Quail LLC
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

void RegisterPxRigidStatic( melua::ScriptEngine * luaSE, me::IGame * game );
