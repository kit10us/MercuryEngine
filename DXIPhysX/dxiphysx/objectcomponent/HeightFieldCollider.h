// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXIPhysX.h>
#include <dxiphysx/GameComponent.h>
#include <dxiphysx/objectcomponent/ColliderBase.h>
#include <dxi/Terra.h>
#include <unify/RowColumn.h>

namespace dxiphysx
{
	namespace objectcomponent
	{
		class HeightFieldCollider : public dxiphysx::objectcomponent::ColliderBase
		{
		public:
			DXIPHYSX_API HeightFieldCollider( me::IOS * os, GameComponent * gameComponent, dxi::Terra * terra, unify::RowColumn< unsigned int > rcCount );
			~HeightFieldCollider();

			std::string GetName() const override;

			me::IObjectComponent * Duplicate() override;
		};
	}
}