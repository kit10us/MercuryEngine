// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <mephysx/objectcomponent/ColliderBase.h>

struct PxShapeProxy
{
	mephysx::objectcomponent::ColliderBase::ptr component;
	physx::PxShape * shape;
};

PxShapeProxy* CheckPxShape( lua_State* state, int index );
int PushPxShape( lua_State * state, mephysx::objectcomponent::ColliderBase::ptr component );

void RegisterPxShape( melua::ScriptEngine * luaSE, me::IGame * game );
