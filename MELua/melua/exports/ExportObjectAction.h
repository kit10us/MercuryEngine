// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <me/object/action/IObjectAction.h>

namespace melua
{
	class ObjectActionProxy
	{
	public:
		MELUADLL_API static char* Name();

		ObjectActionProxy( me::object::action::IObjectAction::ptr action );

		MELUADLL_API me::object::action::IObjectAction::ptr GetAction();

		~ObjectActionProxy();

	private:
		me::object::action::IObjectAction::ptr m_action;
	};

	void RegisterObjectAction( lua_State * state );
}
