// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/object/ObjectComponent.h>
#include <unify/Matrix.h>

namespace adventure
{
	class InventoryObjectComponent : public me::object::ObjectComponent
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