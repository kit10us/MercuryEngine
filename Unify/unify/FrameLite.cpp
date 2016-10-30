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
	m_q = QuaternionLookAt( m_p, at, up );
}

// Move ou position by an amount
void FrameLite::MoveBy( const V3< float > & by )
{
	SetPosition( GetPosition() + by );
}

void FrameLite::Orbit( const V3< float > & origin, const V2< float > & direction, Angle angle )
{
	// Re-origin our position...
	V3< float > newPosition( m_p - origin );

	Quaternion q = Quaternion( V3< float >( direction.y, direction.x, 0 ), angle );

	newPosition = newPosition * q;
	
	m_p = newPosition + origin;
}

void FrameLite::Orbit( const V3< float > & origin, const Quaternion & orbit )
{
	m_p -= origin;
	m_p = m_p * orbit;
	m_p += origin;
}

void FrameLite::PreMul( unify::Quaternion q )
{
	m_q = q * m_q;
}

void FrameLite::PostMul( unify::Quaternion q )
{
	m_q = m_q * q;
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
