// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <mephy/collider/ColliderBase.h>
#include <melua/ScriptEngine.h>

namespace mephylua
{
	void RegisterColliderObjectComponent( melua::ScriptEngine * luaSE );

	class ColliderObjectComponentProxy
	{
	public:
		static char* Name();

		ColliderObjectComponentProxy( me::object::IObjectComponent::ptr component );

		~ColliderObjectComponentProxy();

		me::object::IObjectComponent::ptr component;
		mephy::collider::ColliderBase* collider;
	};
}