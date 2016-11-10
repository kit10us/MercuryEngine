// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxiphysx/objectcomponent/ColliderBase.h>

namespace dxiphysx
{
	namespace objectcomponent
	{
		class CapsuleCollider : public objectcomponent::ColliderBase
		{
		public:
			CapsuleCollider( me::IOS * os, GameComponent * gameComponent, float radius, float halfHeight );
			~CapsuleCollider();

			std::string GetName() const override;

			me::IObjectComponent * Duplicate() override;
		};
	}
}