// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <unify/FrameLite.h>

using namespace unify;

FrameLite::FrameLite()
	: m_q( QuaternionIdentity() )
	, m_p( 0, 0, 0 )
{
}

FrameLite::FrameLite( unify::Quaternion q, unify::V3< float > p )
	: m_q( q )
	, m_p( p )
{
}

FrameLite::~FrameLite()
{
}

// Set us up as a rotation matrix that looks at another frame's position, from our current position
void FrameLite::LookAt( const FrameLite & frame, const V3< float > & up )
{
	LookAt( frame.GetMatrix().GetPosition(), up );
}

void FrameLite::LookAt( const V3< float > & at, const V3< float > & up )
{	
	/*
	V3< float > atFinal = at;
	V3< float > eyePosition = m_p;

	Matrix matrix = unify::Matrix::MatrixIdentity();
	unify::V3< float > forward( at - eyePosition );
	forward.Normalize();

	unify::V3< float > left( unify::V3< float >::V3Cross( up, forward ) );

	SetForward( forward );
	SetLeft( left );
	unify::V3< float > orientedUp = unify::V3< float >::V3Cross( forward, left ); // As the specified parameter up means the world's up, this is the relative up for the look at.
	SetUp( orientedUp ); // Regenerate up.
	//matrix.SetPosition( -left.Dot( eyePosition ), -up.Dot( eyePosition ), -forward.Dot( eyePosition ) );
	SetPosition( eyePosition );
	*/

	V3< float > forward( 0, 0, 1 );

	V3< float > direction = V3< float >::V3Normalized( at - m_p );

	float dot = forward.Dot( direction );

	if ( abs( dot - (-1.0f) ) < 0.000001f )
	{
		m_q = Quaternion( up.x, up.y, up.z, 3.1415926535897932f );
	}
	if ( abs( dot - (1.0f) ) < 0.000001f )
	{
		m_q = QuaternionIdentity();
	}

	Angle rotAngle( AngleInRadians( acos( dot ) ) );
	V3< float > rotAxis = V3< float >::V3Cross( forward, direction );
	rotAxis = V3< float >::V3Normalized( rotAxis );
	m_q = Quaternion( rotAxis, rotAngle );
}

// Move ou position by an amount
void FrameLite::MoveBy( const V3< float > & by )
{
	SetPosition( GetPosition() + by );
}

void FrameLite::Orbit( const V3< float > & origin, const V2< float > & direction, Angle angle )
{
	// Compute our coordinates, so all of our math is in model space.
	V3< float > relativeOrigin = origin;

	// Re-origin our position...
	V3< float > newPosition( GetPosition() - relativeOrigin );
	
	// Create a matrix that has our position as it's Z axis, then rotate that axis...
	Quaternion m(
		Quaternion( V3< float >( 1, 0, 0 ), angle * direction.y ) *
		Quaternion( V3< float >( 0, 1, 0 ), angle * direction.x ) );
	m_q = m;
}

void FrameLite::Orbit( const V3< float > & origin, const Quaternion & orbit, float distance )
{
	V3< float > position( distance, 0, 0 );
	orbit.TransformVector( position );
	position += origin;
	SetPosition( position );
}

void FrameLite::Rotate( unify::Quaternion q )
{
	m_q *= q;
}

Matrix FrameLite::GetMatrix() const
{
	return Matrix( m_q, m_p );
}

V3< float > FrameLite::GetLeft() const
{
	return m_q * V3< float >( 1, 0, 0 );
}

V3< float > FrameLite::GetUp() const
{
	return m_q * V3< float >( 0, 1, 0 );
}

V3< float > FrameLite::GetForward() const
{
	return m_q * V3< float >( 0, 0, -1 );
}

Quaternion FrameLite::GetRotation() const
{
	return m_q;
}

V3< float > FrameLite::GetPosition() const
{
	return m_p;
}

void FrameLite::SetRotation( const Quaternion & rotation )
{
	m_q = rotation;
}			

void FrameLite::SetPosition( const V3< float > & position )
{
	m_p = position;
}
