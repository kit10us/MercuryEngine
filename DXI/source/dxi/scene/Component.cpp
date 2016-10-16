// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/Component.h>

using namespace dxi;
using namespace scene;

Component::Component( core::IOS * os, std::string name )
: m_os( os )
, m_name( name )
{
}

Component::~Component()
{
}

core::IOS * Component::GetOS()
{
	return m_os;
}

const core::IOS * Component::GetOS() const
{
	return m_os;
}

std::string Component::GetName() const
{
	return m_name;
}

void Component::SetName( std::string name )
{
	m_name = name;
}

bool Component::IsEnabled() const
{
	return m_enabled;
}

void Component::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}
