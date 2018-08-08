// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <InventoryObjectComponent.h>

using namespace me;
using namespace object;

using namespace adventure;

InventoryObjectComponent::InventoryObjectComponent( InventoryObjectComponent & component )
	: ObjectComponent( component )
{
}

InventoryObjectComponent::InventoryObjectComponent()
	: ObjectComponent( "Inventory", false, false )
{
}

InventoryObjectComponent::~InventoryObjectComponent()
{
}

component::IObjectComponent::ptr InventoryObjectComponent::Duplicate()
{
	auto duplicate = new InventoryObjectComponent( *this );
	return me::object::component::IObjectComponent::ptr( duplicate );
}
