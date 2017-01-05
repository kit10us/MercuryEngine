// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXIPhysX.h>
#include <dxiphysx/SceneComponent.h>
#include <dxiphysx/objectcomponent/ControllerBase.h>

namespace dxiphysx
{
	namespace objectcomponent
	{
		class CapsuleController : public objectcomponent::ControllerBase
		{
		private:
			CapsuleController( CapsuleController & controller );

		public:
			DXIPHYSX_API CapsuleController( me::IOS * os, SceneComponent * sceneComponent, float radius, float height );
			~CapsuleController();

			std::string GetName() const override;

			me::scene::IObjectComponent * Duplicate() override;

			void OnAttach( me::scene::Object * object ) override;
		};
	}
}