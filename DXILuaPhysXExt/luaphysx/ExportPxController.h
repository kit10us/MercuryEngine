// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <dxiphysx/objectcomponent/ControllerBase.h>

struct PxControllerProxy
{
	me::scene::IObjectComponent::ptr component;
	dxiphysx::objectcomponent::ControllerBase * controller;
};

PxControllerProxy* CheckPxController( lua_State* state, int index );
int PushPxController( lua_State * state, dxiphysx::objectcomponent::ControllerBase::ptr component );

void RegisterPxController( dxilua::ScriptEngine * luaSE, me::IGame * game );
