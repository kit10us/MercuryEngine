// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/Game.h>

void RegisterInput( lua_State * state );

struct InputProxy
{
	me::input::IInputDevice::ptr input;
};

int PushInput( lua_State * state, me::input::IInputDevice::ptr inputSource );
InputProxy* CheckInput( lua_State* state, int index );


