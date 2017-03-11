// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MEPhysX.h>
#include <mephysx/SceneComponent.h>
#include <mephysx/objectcomponent/ControllerBase.h>

namespace mephysx
{
	namespace objectcomponent
	{
		class CapsuleController : public objectcomponent::ControllerBase
		{
		private:
			CapsuleController( CapsuleController & controller );

		public:
			MEPHYSX_API CapsuleController( SceneComponent * sceneComponent, float radius, float height );
			~CapsuleController();

			std::string GetWhat() const override;
			me::scene::IObjectComponent * Duplicate() override;

			void OnAttach( me::scene::Object * object ) override;
		};
	}
}