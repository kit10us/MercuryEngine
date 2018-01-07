// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <mephy/phy2d/SceneComponent2D.h>

using namespace mephy;
using namespace phy2d;

Entity::Entity()
	: ObjectComponent( "MePhysicsEntity", true, false )
{
}

Entity::Entity( Entity & entity )
	: ObjectComponent( entity )
{
}

Entity::~Entity()
{
}

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

MEPHYSICS_API void Entity::AttachEntityOn( Entity * entity )
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
