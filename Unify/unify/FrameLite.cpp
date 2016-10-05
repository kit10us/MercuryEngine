// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <unify/FrameLite.h>

using namespace unify;

FrameLite::FrameLite()
: m_matrix( Matrix::MatrixIdentity() )
{
}

FrameLite::~FrameLite()
{
}

void FrameLite::SetMatrix( const Matrix & matrix )
{
	m_matrix = matrix;
}

// Set us up as a rotation matrix that looks at another frame's position, from our current position
void FrameLite::LookAt( const FrameLite & frame, const V3< float > & up )
{
	LookAt( frame.GetMatrix().GetPosition(), up );
}

void FrameLite::LookAt( const V3< float > & at, const V3< float > & up )
{	
	V3< float > atFinal = at;

	V3< float > eyePosition = m_matrix.GetPosition();

	Matrix matrix = unify::Matrix::MatrixIdentity();
	unify::V3< float > forward( at - eyePosition );
	forward.Normalize();

	unify::V3< float > left( unify::V3< float >::V3Cross( up, forward ) );

	matrix.SetForward( forward );
	matrix.SetLeft( left );
	unify::V3< float > orientedUp = unify::V3< float >::V3Cross( forward, left ); // As the specified parameter up means the world's up, this is the relative up for the look at.
	matrix.SetUp( orientedUp ); // Regenerate up.
	//matrix.SetPosition( -left.Dot( eyePosition ), -up.Dot( eyePosition ), -forward.Dot( eyePosition ) );
	matrix.SetPosition( eyePosition );

	m_matrix = matrix;
}

// Make us look in a given direction (given as a unit vector) (rotation)...
// Requires setting our AT to given DIRECTION, our LEFT to given LEFT, and solving for our UP...
void FrameLite::LookDirection( const V3< float > & direction, const V3< float > & left )
{
	V3< float > directionNormalized = direction;
	V3< float > leftNormalized = left;
	directionNormalized.Normalize();
	leftNormalized.Normalize();
	m_matrix.SetForward( direction );
	m_matrix.SetLeft( left );

	// Figure our UP...
	V3< float > up;
	up.V3Cross( directionNormalized, leftNormalized );
	up.Normalize();
	m_matrix.SetUp( up );
}

// Move ou position by an amount
void FrameLite::MoveBy( const V3< float > & by )
{
	m_matrix.SetPosition( m_matrix.GetPosition() + by );
}

// Slide along an axis (relative to our axis, thus +Z is forward)
void FrameLite::Slide( const V3< float > & axis, float amount )
{
	V3< float > vTransAxis( axis );
	m_matrix.TransformNormal( vTransAxis );
	m_matrix.SetPosition( m_matrix.GetPosition() + (vTransAxis * amount) );
}

void FrameLite::Orbit( const V3< float > & origin, const V2< float > & direction, Angle angle )
{
	// Compute our coordinates, so all of our math is in model space.
	unify::V3< float > relativeOrigin = origin;

	// Re-origin our position...
	unify::V3< float > newPosition( GetMatrix().GetPosition() - relativeOrigin );
	
	// Create a matrix that has our position as it's Z axis, then rotate that axis...
	unify::Matrix m;
	unify::Matrix rotX = unify::Matrix::MatrixRotationX( angle * direction.y );
	unify::Matrix rotY = unify::Matrix::MatrixRotationY( angle * direction.x );
	m = rotX * rotY;
	m_matrix = m_matrix * m;
}

void FrameLite::Orbit( const V3< float > & origin, const Quaternion & orbit, float distance )
{
	V3< float > position( distance, 0, 0 );
	orbit.TransformVector( position );
	position += origin;
	SetPosition( position );
}

void FrameLite::Rotate( const V3< float > & axis, Angle angle )
{
	Matrix matrix = unify::Matrix::MatrixRotationAboutAxis( axis, angle );
	m_matrix = matrix * m_matrix;
}

void FrameLite::Rotate( const Quaternion & q )
{
	Matrix matrix( q );
	m_matrix = matrix * m_matrix;
}

void FrameLite::RotateAbout( const V3< float > & axis, Angle angle )
{
	Matrix mRot = Matrix::MatrixRotationAboutAxis( axis, angle );	
	m_matrix *= mRot;
}

const Matrix & FrameLite::GetMatrix() const
{
	return m_matrix;
}

V3< float > FrameLite::GetLeft() const
{
	return m_matrix.GetLeft();
}

V3< float > FrameLite::GetUp() const
{
	return m_matrix.GetUp();
}

V3< float > FrameLite::GetForward() const
{
	return m_matrix.GetForward();
}

Quaternion FrameLite::GetRotation() const
{
	return m_matrix.GetRotation();
}

V3< float > FrameLite::GetPosition() const
{
	return m_matrix.GetPosition();
}

void FrameLite::SetLeft( const V3< float > & left )
{
	m_matrix.SetLeft( left );
}

void FrameLite::SetUp( const V3< float > & up )
{
	m_matrix.SetUp( up );
}

void FrameLite::SetForward( const V3< float > & direction )
{
	m_matrix.SetForward( direction );
}

void FrameLite::SetRotation( const Quaternion & rotation )
{
	m_matrix = Matrix( rotation, GetPosition(), unify::V3< float >( 1, 1, 1 ) );
}			

void FrameLite::SetPosition( const V3< float > & position )
{
	m_matrix.SetPosition( position );
}

void FrameLite::Set( const Quaternion & orientation, const V3< float > & position, const V3< float > & scale )
{
	m_matrix = Matrix( orientation, position, scale );
}
