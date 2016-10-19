// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxiphysx/objectcomponent/ColliderBase.h>

namespace dxiphysx
{
	namespace objectcomponent
	{
		class SphereCollider : public objectcomponent::ColliderBase
		{
		public:
			SphereCollider( dxi::core::IOS * os, physx::PxPhysics * physics, float radius );
			~SphereCollider();

			std::string GetName() const override;
		};
	}
}