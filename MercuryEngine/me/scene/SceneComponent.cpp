// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/SceneComponent.h>

using namespace me;
using namespace scene;

SceneComponent::SceneComponent( IOS * os, std::string name )
: m_os( os )
, m_name( name )
{
}

SceneComponent::~SceneComponent()
{
}

IOS * SceneComponent::GetOS()
{
	return m_os;
}

const IOS * SceneComponent::GetOS() const
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
