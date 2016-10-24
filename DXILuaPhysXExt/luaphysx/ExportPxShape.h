// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <dxiphysx/objectcomponent/ColliderBase.h>

struct PxShapeProxy
{
	dxiphysx::objectcomponent::ColliderBase::ptr component;
	physx::PxShape * shape;
};

PxShapeProxy* CheckPxShape( lua_State* state, int index );
int PushPxShape( lua_State * state, dxiphysx::objectcomponent::ColliderBase::ptr component );

void RegisterPxShape( dxilua::ScriptEngine * luaSE, dxi::core::IGame * game );
