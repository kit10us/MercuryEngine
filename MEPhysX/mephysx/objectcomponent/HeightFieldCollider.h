// Copyright (c) 2002 - 2013, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <MEPhysX.h>
#include <mephysx/GameComponent.h>
#include <mephysx/objectcomponent/ColliderBase.h>
#include <me/Terra.h>
#include <unify/RowColumn.h>

namespace mephysx
{
	namespace objectcomponent
	{
		class HeightFieldCollider : public mephysx::objectcomponent::ColliderBase
		{
		public:
			MEPHYSX_API HeightFieldCollider( GameComponent * gameComponent, me::Terra * terra, unify::RowColumn< unsigned int > rcCount );
			~HeightFieldCollider();

			std::string GetWhat() const override;

			me::object::IObjectComponent::ptr Duplicate() override;
		};
	}
}