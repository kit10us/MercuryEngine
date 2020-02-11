// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once 

#include <me/phy/Entity.h>
#include <me/scene/component/SceneComponent.h>

namespace me
{
	namespace phy
	{
		class PhySceneComponent : public me::scene::component::SceneComponent
		{
		public:
			PhySceneComponent( me::os::IOS * os, std::string name );
			~PhySceneComponent();

			virtual Entity* AddEntity( me::object::Object * object ) = 0;
		};
	}
}
