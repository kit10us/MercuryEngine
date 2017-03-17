// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <FollowComponent.h>
#include <me/object/Object.h>

using namespace me;

FollowComponent::FollowComponent( FollowComponent & component )
	: object::ObjectComponent( component )
	, m_target{ component.m_target }
	, m_offset{ component.m_offset }
{
}

FollowComponent::FollowComponent()
	: object::ObjectComponent( "Follow" )
	, m_target{ nullptr }
	, m_offset{ unify::MatrixIdentity() }
{
}

FollowComponent::~FollowComponent() 
{
}

void FollowComponent::SetTarget( me::object::Object * target )
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

object::IObjectComponent::ptr FollowComponent::Duplicate()
{
	auto component = new FollowComponent( *this );
	return object::IObjectComponent::ptr( component );
}

std::string FollowComponent::GetWhat() const
{
	return std::string();
}								
