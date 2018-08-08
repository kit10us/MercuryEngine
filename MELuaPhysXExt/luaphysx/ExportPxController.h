// Copyright (c) 2002 - 2011, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <mephysx/objectcomponent/ControllerBase.h>

struct PxControllerProxy
{
	me::object::IObjectComponent::ptr component;
	mephysx::objectcomponent::ControllerBase * controller;
};

PxControllerProxy* CheckPxController( lua_State* state, int index );
int PushPxController( lua_State * state, mephysx::objectcomponent::ControllerBase::ptr component );

void RegisterPxController( melua::ScriptEngine * luaSE, me::IGame * game );
