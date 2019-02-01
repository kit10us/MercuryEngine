// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <mephy/phy3d/collider/ColliderBase3D.h>
#include <melua/ScriptEngine.h>

namespace mephylua
{
	void RegisterColliderObjectComponent( melua::ScriptEngine * luaSE );

	class ColliderObjectComponentProxy
	{
	public:
		static char* Name();

		ColliderObjectComponentProxy( me::object::component::IObjectComponent::ptr component );

		~ColliderObjectComponentProxy();

		me::object::component::IObjectComponent::ptr component;
		mephy::phy3d::collider::ColliderBase* collider;
	};
}