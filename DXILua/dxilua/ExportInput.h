// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <dxi/core/Game.h>

void RegisterInput( lua_State * state );

struct InputProxy
{
	dxi::input::IInputSource::ptr input;
};

InputProxy* CheckInput( lua_State* state, int index );


