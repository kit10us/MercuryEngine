// Copyright (c) 2002 - 2018, Kit10 Studios LLC
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
