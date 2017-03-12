// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <FollowComponent.h>
#include <me/scene/Object.h>

using namespace me;

FollowComponent::FollowComponent()
	: scene::ObjectComponent( "Follow" )
	, m_target{ nullptr }
	, m_offset{ unify::MatrixIdentity() }
{
}

FollowComponent::~FollowComponent() 
{
}

void FollowComponent::SetTarget( me::scene::Object * target )
{
	m_target = target;
}

void FollowComponent::SetOffset( unify::Matrix offset )
{
	m_offset = offset;
}

void FollowComponent::OnUpdate( UpdateParams params )
{
	if ( ! IsEnabled() )
	{
		return;
	}			

	if ( m_target )
	{
		GetObject()->GetFrame().SetPosition( m_target->GetFrame().GetPosition() + m_offset.GetPosition() );
	}
}

scene::IObjectComponent * FollowComponent::Duplicate()
{
	return nullptr;
}

std::string FollowComponent::GetWhat() const
{
	return std::string();
}								
