// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <mephy/Entity.h>

using namespace me;
using namespace mephy;

char* Entity::Name()
{
	return "MEPhyiscsEntity";
}

Entity::Entity( Entity & component )
	: ObjectComponent( component )
{
}

Entity::Entity()
	: ObjectComponent( Name(), false, false )
{
}

Entity::~Entity()
{
}

void Entity::CaptureEarly()
{
	m_early = GetObject()->GetFrame().GetMatrix();
}

const unify::Matrix & Entity::GetEarly() const
{
	return m_early;
}

me::object::IObjectComponent::ptr Entity::Duplicate()
{
	return me::object::IObjectComponent::ptr( new Entity( *this ) );
}
