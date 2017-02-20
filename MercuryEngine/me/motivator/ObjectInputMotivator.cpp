// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/motivator/ObjectInputMotivator.h>
#include <unify/Angle.h>
#include <unify/V3.h>
#include <unify/Quaternion.h>

using namespace me;
using namespace motivator;
using namespace input;

ObjectInputMotivator::ObjectInputMotivator()
{
}

ObjectInputMotivator::~ObjectInputMotivator() 
{
}

std::string ObjectInputMotivator::GetName() const
{
	return "ObjectInputMotivator";
}

bool ObjectInputMotivator::IsEnabled() const
{
	return m_enabled;
}

void ObjectInputMotivator::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

bool ObjectInputMotivator::Updateable() const
{
	return true;
}

bool ObjectInputMotivator::Renderable() const
{
	return false;
}

void ObjectInputMotivator::OnAttach( scene::Object * object )
{
	m_target = object;
}

void ObjectInputMotivator::OnDetach()
{
	m_target = nullptr;
}

void ObjectInputMotivator::OnInit()
{
}

void ObjectInputMotivator::OnStart()
{
}

void ObjectInputMotivator::OnUpdate( IRenderer * renderer, const RenderInfo & renderInfo )
{
	if ( ! m_target )
	{
		return;
	}

	if ( ! m_enabled )
	{
		return;
	}


	auto && runOnMotivation = m_motivations[ "runOn" ];
	auto && strafeLeftMotivation = m_motivations[ "strafeleft" ];
	auto && strafeRightMotivation = m_motivations[ "straferight" ];
	auto && walkForwardMotivation = m_motivations[ "walkforward" ];
	auto && walkBackwardMotivation = m_motivations[ "walkbackward" ];	 

	float speed = 1.0f;
	if ( (*runOnMotivation)() )
	{
		speed = 3.0f;
	}

	if ( (*strafeLeftMotivation)() )
	{
		m_target->GetFrame().MoveBy( unify::V3< float >( -1, 0, 0 ) * renderInfo.GetDelta() * speed );
	}
	else if ( (*strafeRightMotivation)() )
	{
		m_target->GetFrame().MoveBy( unify::V3< float >( 1, 0, 0 ) * renderInfo.GetDelta() * speed );
	}

	if ( (*walkForwardMotivation)() )
	{
		m_target->GetFrame().MoveBy( unify::V3< float >( 0, 0, 1 ) * renderInfo.GetDelta() * speed );
	}
	else if ( (*walkBackwardMotivation)() )
	{
		m_target->GetFrame().MoveBy( unify::V3< float >( 0, 0, -1 ) * renderInfo.GetDelta() * speed );
	}
}

void ObjectInputMotivator::CollectGeometry( GeometryCache & cache, const unify::FrameLite * frame )
{
}

void ObjectInputMotivator::OnSuspend()
{
}

void ObjectInputMotivator::OnResume()
{
}

scene::IObjectComponent * ObjectInputMotivator::Duplicate()
{
	return nullptr; // Not supported.
}

void ObjectInputMotivator::Add( std::string motivation, IInputCondition::ptr condition )
{
	m_motivations[ motivation ] = condition;
}
