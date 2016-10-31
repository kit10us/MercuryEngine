// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/ObjectComponent.h>

using namespace dxi;
using namespace scene;

ObjectComponent::ObjectComponent( ObjectComponent & component )
	: m_os( component.m_os )
{
}

ObjectComponent::ObjectComponent( core::IOS * os )
: m_os( os )
{
}

ObjectComponent::~ObjectComponent()
{
}

core::IOS * ObjectComponent::GetOS()
{
	return m_os;
}

const core::IOS * ObjectComponent::GetOS() const
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
