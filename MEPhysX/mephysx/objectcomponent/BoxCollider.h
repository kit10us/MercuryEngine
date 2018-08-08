// Copyright (c) 2002 - 2013, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <MEPhysX.h>
#include <mephysx/GameComponent.h>
#include <mephysx/objectcomponent/ColliderBase.h>

namespace mephysx
{
	namespace objectcomponent
	{
		class BoxCollider : public objectcomponent::ColliderBase
		{
		private:
			BoxCollider( BoxCollider & collider );

		public:
			MEPHYSX_API BoxCollider( GameComponent * gameComponent, unify::V3< float > halfExt );
			~BoxCollider();

			std::string GetWhat() const override;

			me::object::IObjectComponent::ptr Duplicate() override;
		};
	}
}