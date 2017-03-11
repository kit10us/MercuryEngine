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
		class CapsuleCollider : public objectcomponent::ColliderBase
		{
		private:
			CapsuleCollider( CapsuleCollider & collider );

		public:
			MEPHYSX_API CapsuleCollider( GameComponent * gameComponent, float radius, float halfHeight );
			~CapsuleCollider();

			std::string GetWhat() const override;

			me::scene::IObjectComponent * Duplicate() override;
		};
	}
}