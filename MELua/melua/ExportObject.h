// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <me/object/Object.h>

namespace melua
{
	struct ObjectProxy
	{
		static char* Name();
		me::object::Object * object;
	};

	void RegisterObject( lua_State * state );
}
