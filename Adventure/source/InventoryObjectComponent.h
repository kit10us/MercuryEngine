// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/object/component/ObjectComponent.h>
#include <unify/Matrix.h>

namespace adventure
{
	class InventoryObjectComponent : public me::object::component::ObjectComponent
	{
	protected:
		InventoryObjectComponent( InventoryObjectComponent & component );

	public:
		InventoryObjectComponent();
		virtual ~InventoryObjectComponent();

		IObjectComponent::ptr Duplicate() override;

	protected:

	};
}