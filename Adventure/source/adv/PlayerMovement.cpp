// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <adv/PlayerMovement.h>

using namespace adv;

PlayerMovement::PlayerMovement( float speed )
: m_newMovement{ false }
, m_movementDirection{ unify::V3< float >::V3Zero() }
, m_speed{ speed }
{
}

void PlayerMovement::Perform( MovementDirection direction )
{
	m_newMovement = true;

	switch( direction )
	{
	case MovementDirection::Up:
		m_movementDirection.z = 1.0f;
		break;
	case MovementDirection::Down:
		m_movementDirection.z = -1.0f;
		break;
	case MovementDirection::Left:
		m_movementDirection.x = -1.0f;
		break;
	case MovementDirection::Right:
		m_movementDirection.x = 1.0f;
		break;
	}

	// Normalize, so if we move in an angle, we aren't moving twice as fast.
	m_movementDirection.Normalize();
}


void PlayerMovement::Perform( unify::V3< float > direction )
{
	// Normalize, so if we move in an angle, we aren't moving twice as fast.
	float length = direction.Normalize();
	if (length > 0.2f)
	{
		m_newMovement = true;
		m_movementDirection = direction;
	}
}

void PlayerMovement::Update( me::object::Object* target,  const me::UpdateParams & params )
{
	if ( !m_newMovement )
	{
		return;
	}

	float factor = params.GetDelta().GetSeconds() * m_speed;
	m_movementDirection *= factor;

	// Determine what direction we are facing
	unify::Angle direction = m_movementDirection.DotAngle( { 0.0f, 0.0f, 1.0f } );

	// Account for the left (-x) inversing the direction
	if ( m_movementDirection.x < 0.0f )
	{
		direction *= -1.0f;
	}

	// Reset our rotation to identity (facing up the z-axis).
	target->GetFrame().SetRotation( unify::QuaternionIdentity() );

	// Move our position...
	unify::V3< float > position = target->GetFrame().GetPosition();
	position += m_movementDirection;

	target->GetFrame().SetPosition( position );

	// Face the correct direction.
	target->GetFrame().SetRotation( unify::MatrixRotationY( direction ) );

	// Reset move variables...
	m_newMovement = false;
	m_movementDirection = unify::V3< float >::V3Zero();
}

