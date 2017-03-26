// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/action/IAction.h>

struct ActionProxy
{
	MELUADLL_API static char* Name();
	me::action::IAction::ptr action;
};

void RegisterAction( lua_State * state );

