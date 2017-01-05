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
		class CapsuleCollider : public objectcomponent::ColliderBase
		{
		private:
			CapsuleCollider( CapsuleCollider & collider );

		public:
			DXIPHYSX_API CapsuleCollider( me::IOS * os, GameComponent * gameComponent, float radius, float halfHeight );
			~CapsuleCollider();

			std::string GetName() const override;

			me::scene::IObjectComponent * Duplicate() override;
		};
	}
}