// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <PlayerMovementStick.h>

using namespace me;
using namespace render;
using namespace scene;
using namespace object;

PlayerMovementStick::PlayerMovementStick( Object * object )
	: m_object{ object }
{
}

bool PlayerMovementStick::Perform( input::IInputDevice * device, input::IInputCondition * condition, float delta )
{
	float speed = 6.0f;

	unify::V3< float > move = condition->GetValue( device ).xzy();

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
