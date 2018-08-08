// Copyright (c) 2002 - 2013, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <MEPhysX.h>
#include <me/object/ObjectComponent.h>
#include <mephysx/SceneComponent.h>

namespace mephysx
{
	namespace objectcomponent
	{
		class ControllerBase : public me::object::ObjectComponent
		{
		protected:
			ControllerBase( ControllerBase & controller );

		public:
			ControllerBase( std::string typeName, SceneComponent * gameComponent );
			~ControllerBase();

			MEPHYSX_API physx::PxController * GetController();
			MEPHYSX_API const physx::PxController * GetController() const;

			MEPHYSX_API SceneComponent * GetScene();
			MEPHYSX_API const SceneComponent * GetScene() const;

		protected:
			SceneComponent * m_sceneComponent;
			std::shared_ptr< physx::PxController > m_controller;
		};
	}
}