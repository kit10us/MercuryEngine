// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once 

#include <me/phy/Entity.h>
#include <me/scene/SceneComponent.h>

namespace me
{
	namespace phy
	{
		class PhySceneComponent : public me::scene::SceneComponent
		{
		public:
			PhySceneComponent( me::os::IOS * os, std::string name );
			~PhySceneComponent();

			virtual Entity* AddEntity( me::object::Object * object ) = 0;
		};
	}
}
