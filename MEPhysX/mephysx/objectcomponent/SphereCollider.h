// Copyright (c) 2002 - 2013, Quentin S. Smith
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
			MEPHYSX_API SphereCollider( me::IOS * os, GameComponent * gameComponent, float radius );
			~SphereCollider();

			std::string GetType() const override;
			std::string GetWhat() const override;

			me::scene::IObjectComponent * Duplicate() override;
		};
	}
}