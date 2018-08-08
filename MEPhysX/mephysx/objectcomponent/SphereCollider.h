// Copyright (c) 2002 - 2013, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <mephysx/objectcomponent/ColliderBase.h>

namespace mephysx
{
	namespace objectcomponent
	{
		class SphereCollider : public objectcomponent::ColliderBase
		{
		public:
			MEPHYSX_API SphereCollider( GameComponent * gameComponent, float radius );
			~SphereCollider();

			std::string GetWhat() const override;

			me::object::IObjectComponent::ptr Duplicate() override;
		};
	}
}