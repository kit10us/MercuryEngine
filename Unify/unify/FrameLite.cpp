// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <unify/FrameLite.h>

using namespace unify;

FrameLite::FrameLite()
	: m_q( QuaternionIdentity() )
	, m_p( 0, 0, 0 )
	, m_mat{ m_q, m_p }
	, m_useModelMatrix( false )
	, m_modelMatrix( MatrixIdentity() )
{
}

FrameLite::FrameLite( unify::Quaternion q, unify::V3< float > p )
	: m_q( q )
	, m_p( p )
	, m_mat{ m_q, m_p }
	, m_useModelMatrix( false )
	, m_modelMatrix( MatrixIdentity() )
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
	m_mat = Matrix{ m_q, m_p };
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

	m_mat = Matrix{ m_q, m_p };
}

void FrameLite::Orbit( const V3< float > & origin, const Quaternion & orbit )
{
	m_p -= origin;
	m_p = m_p * orbit;
	m_p += origin;

	m_mat = Matrix{ m_q, m_p };
}

void FrameLite::PreMul( Quaternion q )
{
	m_q = q * m_q;
	m_mat = Matrix{ m_q, m_p };
}

void FrameLite::PostMul( Quaternion q )
{
	m_q = m_q * q;
	m_mat = Matrix{ m_q, m_p };
}

void FrameLite::PreMul( Matrix m )
{
	Matrix mine { m_q, m_p };
	mine = m * mine;
	m_q = mine.GetRotation();
	m_p = mine.GetPosition();
	m_mat = Matrix{ m_q, m_p };
}

void FrameLite::PostMul( Matrix m )
{
	Matrix mine { m_q, m_p };
	mine = mine * m;
	m_q = mine.GetRotation();
	m_p = mine.GetPosition();
	m_mat = Matrix{ m_q, m_p };
}

Matrix FrameLite::GetMatrix() const
{
	if ( m_useModelMatrix )
	{
		//return m_modelMatrix * m_mat;
		return m_modelMatrix * Matrix( m_q, m_p );
	}
	else
	{
		//return m_mat;
		return Matrix( m_q, m_p );
	}
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
	m_mat = Matrix{ m_q, m_p };
}			

void FrameLite::SetPosition( const V3< float > & position )
{
	m_p = position;
	m_mat = Matrix{ m_q, m_p };
}

void FrameLite::SetModelMatrix( Matrix & modelMatrix )
{
	m_useModelMatrix = true;
	m_modelMatrix = modelMatrix;
}

const Matrix & FrameLite::GetModelMatrix() const
{
	return m_modelMatrix;
}

Matrix & FrameLite::GetModelMatrix()
{
	m_useModelMatrix = true;
	return m_modelMatrix;
}

bool FrameLite::GetUseModelMatrix() const
{
	return m_useModelMatrix;
}

