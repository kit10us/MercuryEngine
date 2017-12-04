// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <PlayerMovementButtons.h>

using namespace me;
using namespace render;
using namespace scene;
using namespace object;

PlayerMovementButtons::PlayerMovementButtons( Object * object )
	: m_object{ object }
{
}

bool PlayerMovementButtons::Perform( input::IInputDevice * device, input::IInputCondition * condition, float delta )
{
	if( !condition->IsTrue( device ) )
	{
		return true;
	}
		
	float speed = 6.0f;
	
	unify::V3< float > move{ 0.0f, 0.0f, 0.0f };
	if( unify::StringIs( condition->GetName(), "Up" ) )
	{
		move.z = 1.0f;
	}
	else if( unify::StringIs( condition->GetName(), "Down" ) )
	{
		move.z = -1.0f;
	}
	else if( unify::StringIs( condition->GetName(), "Left" ) )
	{
		move.x = -1.0f;
	}
	else if( unify::StringIs( condition->GetName(), "Right" ) )
	{
		move.x = 1.0f;
	}

	// Normalize, so if we move in an angle, we aren't moving twice as fast.
	move.Normalize();

	// Determine what direction we are facing
	unify::Angle direction = move.DotAngle( { 0.0f, 0.0f, 1.0f } );

	// Account for the left (-x) inversing the direction
	if( move.x < 0.0f )
	{
		direction *= -1.0f;
	}

	// Accumulate our movement speed...
	float factor = delta * speed;
	move *= factor;

	// Reset our rotation to identity (facing up the z-axis).
	m_object->GetFrame().SetRotation( unify::QuaternionIdentity() );

	// Move our position...
	unify::V3< float > position = m_object->GetFrame().GetPosition();
	position += move;

	m_object->GetFrame().SetPosition( position );

	// Face the correct direction.
	m_object->GetFrame().SetRotation( unify::MatrixRotationY( direction ) );

	return true;
}
