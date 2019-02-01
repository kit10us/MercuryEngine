// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/game/Game.h>

void RegisterInput( lua_State * state );

struct InputProxy
{
	static char * Name();
	me::input::IInputDevice::ptr input;
};

int PushInput( lua_State * state, me::input::IInputDevice::ptr inputSource );
InputProxy* CheckInput( lua_State* state, int index );


