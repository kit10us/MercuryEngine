// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/SceneComponent.h>

using namespace dxi;
using namespace scene;

SceneComponent::SceneComponent( core::IOS * os, std::string name )
: m_os( os )
, m_name( name )
{
}

SceneComponent::~SceneComponent()
{
}

core::IOS * SceneComponent::GetOS()
{
	return m_os;
}

const core::IOS * SceneComponent::GetOS() const
{
	return m_os;
}

const char * SceneComponent::GetName() const
{
	return m_name.c_str();
}

bool SceneComponent::IsEnabled() const
{
	return m_enabled;
}

void SceneComponent::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}
