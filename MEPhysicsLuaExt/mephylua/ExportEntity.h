// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <mephy/Entity.h>
#include <melua/ScriptEngine.h>

namespace mephylua
{
	void RegisterEntityObjectComponent( melua::ScriptEngine * luaSE );

	class EntityObjectComponentProxy
	{
	public:
		static char* Name();

		EntityObjectComponentProxy( me::object::component::IObjectComponent::ptr component );

		~EntityObjectComponentProxy();

		me::object::component::IObjectComponent::ptr component;
		mephy::Entity* entity;
	};
}