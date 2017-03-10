// Copyright (c) 2002 - 2013, Quentin S. Smith
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
			MEPHYSX_API BoxCollider( me::IOS * os, GameComponent * gameComponent, unify::V3< float > halfExt );
			~BoxCollider();

			std::string GetType() const override;
			std::string GetWhat() const override;

			me::scene::IObjectComponent * Duplicate() override;
		};
	}
}