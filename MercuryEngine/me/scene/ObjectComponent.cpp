// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/ObjectComponent.h>

using namespace me;
using namespace scene;

ObjectComponent::ObjectComponent( ObjectComponent & component )
	: m_os( component.m_os )
{
}

ObjectComponent::ObjectComponent( IOS * os )
: m_os( os )
{
}

ObjectComponent::~ObjectComponent()
{
}

IOS * ObjectComponent::GetOS()
{
	return m_os;
}

const IOS * ObjectComponent::GetOS() const
{
	return m_os;
}

bool ObjectComponent::IsEnabled() const
{
	return m_enabled;
}

void ObjectComponent::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}
