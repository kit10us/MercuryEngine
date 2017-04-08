// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <lua.hpp>

namespace melua
{
	/// <summary>
	/// Registers Lua libraries, and our class libraries.
	/// </summary>
	void RegisterLibraries( lua_State * state );
}