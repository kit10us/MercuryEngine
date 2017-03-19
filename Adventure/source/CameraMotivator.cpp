// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <CameraMotivator.h>
#include <unify/Angle.h>
#include <unify/V3.h>
#include <unify/Quaternion.h>

using namespace me;
using namespace motivator;
using namespace input;

CameraMotivator::CameraMotivator( const CameraMotivator & cameraMotivator )
	: ObjectInputMotivator( cameraMotivator )
{
}

CameraMotivator::CameraMotivator()
	: ObjectInputMotivator()
{
	m_typeName = "CameraMotivator";
	GetLookup()->AddValue( "speed", "2.0" );
}

CameraMotivator::~CameraMotivator() 
{
}

std::string CameraMotivator::GetWhat() const
{
	return std::string();
}								

void CameraMotivator::OnUpdate( UpdateParams params )
{
	if ( ! IsEnabled() )
	{
		return;
	}								 

	// Digital movement...
	auto && moveLeftMotivation = GetMotivator()->GetCondition( "moveleft" );
	auto && moveRightMotivation = GetMotivator()->GetCondition( "moveright" );
	auto && moveUpMotivation = GetMotivator()->GetCondition( "moveup" );
	auto && moveDownMotivation = GetMotivator()->GetCondition( "movedown" );

	float speed = unify::Cast< float >( GetValue( "speed" ) );

	typedef unify::V3< float > V;

	V move{ V::V3Zero() };
	bool wantMove{};
	
	using namespace unify;
	if ( moveLeftMotivation && moveLeftMotivation->IsTrue() )
	{
		wantMove = true;
		move += {moveLeftMotivation->GetValue(), 0, 0 };
	}
	else if ( moveRightMotivation && moveRightMotivation->IsTrue() )
	{
		wantMove = true;
		move += {moveRightMotivation->GetValue(), 0, 0 };
	}

	if ( moveUpMotivation && moveUpMotivation->IsTrue() )
	{
		wantMove = true;
		move += {0, 0, moveUpMotivation->GetValue() };
	}
	else if ( moveDownMotivation && moveDownMotivation->IsTrue() )
	{
		wantMove = true;
		move += {0, 0, moveDownMotivation->GetValue() };
	}

	if ( wantMove )
	{
		// Normalize, so if we move in an angle, we aren't moving twice as fast.
		move.Normalize();

		// Determine what direction we are facing
		unify::Angle direction = move.DotAngle( { 0.0f, 0.0f, 1.0f } );
		
		// Account for the left (-x) inversing the direction
		if ( move.x < 0.0f )
		{
			direction *= -1.0f;
		}
						  
		// Accumulate our movement speed...
		float factor = params.renderInfo.GetDelta() * speed;
		move *= factor;

		// Reset our rotation to identity (facing up the z-axis).
		GetObject()->GetFrame().SetRotation( QuaternionIdentity() );

		// Move our position...
		V position = GetObject()->GetFrame().GetPosition();
		position += move;
		GetObject()->GetFrame().SetPosition( position );	

		// Face the correct direction.
		GetObject()->GetFrame().SetRotation( unify::MatrixRotationY( direction ) );
	}
}

object::IObjectComponent::ptr CameraMotivator::Duplicate()
{
	auto component = new CameraMotivator( *this );
	return object::IObjectComponent::ptr( component );
}