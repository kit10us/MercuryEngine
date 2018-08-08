// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <melua/ScriptEngine.h>
#include <me/input/IInputAction.h>

struct InputActionProxy
{
	MELUADLL_API static char* Name();
	me::input::IInputAction::ptr action;
};

void RegisterInputAction( lua_State * state );

