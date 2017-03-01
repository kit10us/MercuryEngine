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

	// Digital movement...
	auto && strafeLeftMotivation = m_motivations[ "strafeleft" ];
	auto && strafeRightMotivation = m_motivations[ "straferight" ];
	auto && walkForwardMotivation = m_motivations[ "walkforward" ];
	auto && walkBackwardMotivation = m_motivations[ "walkbackward" ];	 

	// Analog movement...
	auto && walkXMotivation = m_motivations[ "walkx" ];
	auto && walkYMotivation = m_motivations[ "walkY" ];

	// Look...
	auto && lookXMotivation = m_motivations[ "lookX" ];
	auto && lookYMotivation = m_motivations[ "lookY" ];

	float speed = 1.0f;
	if ( runOnMotivation && runOnMotivation->IsTrue() )
	{
		speed = 3.0f;
	}

	if ( strafeLeftMotivation && strafeLeftMotivation->IsTrue() )
	{
		m_target->GetFrame().MoveBy( unify::V3< float >( -1, 0, 0 ) * renderInfo.GetDelta() * speed );
	}
	else if ( strafeRightMotivation && strafeRightMotivation->IsTrue() )
	{
		m_target->GetFrame().MoveBy( unify::V3< float >( 1, 0, 0 ) * renderInfo.GetDelta() * speed );
	}

	if ( walkForwardMotivation && walkForwardMotivation->IsTrue() )
	{
		m_target->GetFrame().MoveBy( unify::V3< float >( 0, 0, 1 ) * renderInfo.GetDelta() * speed );
	}
	else if ( walkBackwardMotivation && walkBackwardMotivation->IsTrue() )
	{
		m_target->GetFrame().MoveBy( unify::V3< float >( 0, 0, -1 ) * renderInfo.GetDelta() * speed );
	}

	if ( walkXMotivation && walkXMotivation->IsTrue() )
	{
		unify::V3< float > left = m_target->GetFrame().GetLeft();
		unify::Matrix matrix( unify::MatrixTranslate( left * walkXMotivation->GetValue() * renderInfo.GetDelta() * speed * 5.0f ) );
		m_target->GetFrame().PostMul( matrix );
	}

	if ( walkYMotivation && walkYMotivation->IsTrue() )
	{
		unify::V3< float > forward = m_target->GetFrame().GetForward();
		unify::Matrix matrix( unify::MatrixTranslate( forward * walkYMotivation->GetValue() * renderInfo.GetDelta() * speed * 5.0f ) );
		m_target->GetFrame().PostMul( matrix );
	}

	bool lookXMotivated = (lookXMotivation && lookXMotivation->IsTrue());
	bool lookYMotivated = (lookYMotivation && lookYMotivation->IsTrue());
	if ( lookXMotivated || lookYMotivated )
	{
		unify::Matrix matrix = unify::MatrixIdentity();

		if ( lookXMotivated )
		{
			float changeX = lookXMotivation->GetValue();
			matrix *= unify::MatrixRotationY( unify::AngleInRadians( renderInfo.GetDelta() * changeX * 0.4f ) );
		}

		if ( lookYMotivated )
		{
			float changeY = lookYMotivation->GetValue();
			matrix *= unify::MatrixRotationX( unify::AngleInRadians( renderInfo.GetDelta() * changeY * 0.4f ) );
		}

		m_target->GetFrame().PreMul( matrix );
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
