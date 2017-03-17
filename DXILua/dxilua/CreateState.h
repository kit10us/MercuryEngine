// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <lua.hpp>

namespace dxilua
{
	/// <summary>
	/// Registers Lua libraries, and our class libraries.
	/// </summary>
	void RegisterLibraries( lua_State * state );
}