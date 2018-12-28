// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/phy/Entity.h>

using namespace me;
using namespace phy;

Entity::Entity( std::string type, bool update, bool render )
	: ObjectComponent( type, update, render )
{
}

Entity::Entity( Entity & entity )
	: ObjectComponent( entity )
{
}

Entity::~Entity()
{
}
/*
void Entity::UpdateEntities()
{
	for( auto * entity : m_attachedEntities )
	{
		entity->MoveTo( unify::V2< float >{ 0, 0 } );
	}
}

void Entity::MoveTo( unify::V3< float > position )
{
	m_object->GetFrame().SetPosition( position );
}

MEPHY2D_API void Entity::AttachEntityOn( Entity * entity )
{
	m_attachedEntities.push_back( entity );
}

void Entity::OnAttach( me::object::Object * object )
{
	m_object = object;
}

void Entity::OnDetach( me::object::Object * objecct )
{
	m_object = nullptr;
}


me::object::component::IObjectComponent::ptr Entity::Duplicate()
{
	auto duplicate = new Entity( *this );
	return me::object::component::IObjectComponent::ptr( duplicate );
}
*/