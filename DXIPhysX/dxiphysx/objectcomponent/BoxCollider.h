// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxiphysx/objectcomponent/ColliderBase.h>

namespace dxiphysx
{
	namespace objectcomponent
	{
		class BoxCollider : public dxiphysx::objectcomponent::ColliderBase
		{
		public:
			BoxCollider( dxi::core::IOS * os, physx::PxPhysics * physics, unify::V3< float > halfExt );
			~BoxCollider();

			std::string GetName() const override;
		};
	}
}