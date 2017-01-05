// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXIPhysX.h>
#include <dxiphysx/GameComponent.h>
#include <dxiphysx/objectcomponent/ColliderBase.h>

namespace dxiphysx
{
	namespace objectcomponent
	{
		class BoxCollider : public objectcomponent::ColliderBase
		{
		private:
			BoxCollider( BoxCollider & collider );

		public:
			DXIPHYSX_API BoxCollider( me::IOS * os, GameComponent * gameComponent, unify::V3< float > halfExt );
			~BoxCollider();

			std::string GetName() const override;

			me::scene::IObjectComponent * Duplicate() override;
		};
	}
}