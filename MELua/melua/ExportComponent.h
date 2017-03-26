// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <me/IComponent.h>

namespace melua
{
	void RegisterComponent( lua_State * state );

	struct ComponentProxy
	{
		MELUADLL_API static char* Name();
		me::IComponent* component;
	};

	MELUADLL_API ComponentProxy* CheckComponent( lua_State* state, int index );
	MELUADLL_API int PushComponent( lua_State * state, me::IComponent* component );
}