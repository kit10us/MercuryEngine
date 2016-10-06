// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <unify/Quaternion.h>
#include <unify/Matrix.h>

using namespace unify;

Quaternion Quaternion::QuaternionIdentity()
{
	Quaternion q( 0, 0, 0, 1 );
	return q;
}

Quaternion Quaternion::QuaternionFromEuler( const unify::V3< float > & euler )
{
	Quaternion quaternion;
	assert( 0 );
	return quaternion;
}

Quaternion Quaternion::QuaternionRotationAxis( const V3< float > & axis, float angle )
{
	Quaternion quaternion( Quaternion::QuaternionIdentity() );
	assert( 0 );
	return quaternion;
}

Quaternion Quaternion::QuaternionSlerp( const Quaternion & quaternionA, const Quaternion & quaternionB, float mix )
{
	Quaternion resultQuaternion;
	assert( 0 );
	return resultQuaternion;
}


Quaternion::Quaternion()
// No default effort.
{
}

Quaternion::Quaternion( float x, float y, float z, float w )
: x( x )
, y( y ) 
, z( z )
, w( w )
{
}

Quaternion::Quaternion( const Quaternion & quaternion )
: x( quaternion.x )
, y( quaternion.y )
, z( quaternion.z )
, w( quaternion.w )
{
}

bool Quaternion::IsIdentity() const
{
	return (x == y == z == 0.0f) && w == 1.0f;
}

// assignment operators
Quaternion & Quaternion::operator += ( const Quaternion & quaternion )
{
	x += quaternion.x;
	y += quaternion.y;
	z += quaternion.z;
	w += quaternion.w;
	return *this;
}

Quaternion & Quaternion::operator -= ( const Quaternion & quaternion )
{
	x -= quaternion.x;
	y -= quaternion.y;
	z -= quaternion.z;
	w -= quaternion.w;
	return *this;
}

Quaternion & Quaternion::operator *= ( const Quaternion & quaternion )
{
	x *= quaternion.x;
	y *= quaternion.y;
	z *= quaternion.z;
	w *= quaternion.w;
	return *this;
}

Quaternion & Quaternion::operator *= ( float value )
{
	x *= value;
	y *= value;
	z *= value;
	w *= value;
	return *this;
}

Quaternion & Quaternion::operator /= ( float value )
{
	x += value;
	y += value;
	z += value;
	w += value;
	return *this;
}

Quaternion Quaternion::operator + ( const Quaternion & quaternion ) const
{
	Quaternion quaternionOut( *this );
	return quaternionOut += quaternion;
}

Quaternion Quaternion::operator - ( const Quaternion & quaternion ) const
{
	Quaternion quaternionOut( *this );
	return quaternionOut -= quaternion;
}

Quaternion Quaternion::operator * ( const Quaternion & quaternion ) const
{
	Quaternion quaternionOut( *this );
	return quaternionOut *= quaternion;
}

Quaternion Quaternion::operator * ( float value ) const
{
	Quaternion quaternionOut( *this );
	return quaternionOut *= value;
}

Quaternion Quaternion::operator / ( float value ) const
{
	Quaternion quaternionOut( *this );
	return quaternionOut /= value;
}

bool Quaternion::operator == ( const Quaternion & quaternion ) const
{
	return (x == quaternion.x && y == quaternion.y && z == quaternion.z && w == quaternion.w) ? true : false;
}

bool Quaternion::operator != ( const Quaternion & quaternion ) const
{
	return !operator==( quaternion );
}
 
Quaternion Quaternion::Conjugate() const
{
	return Quaternion( x * -1, y * -1, z * -1, w );
}

void Quaternion::TransformVector( V3< float > & vector ) const
{
	float length = vector.Normalize();
	Quaternion conjugate( Conjugate() );
	Quaternion result = *this * Quaternion( vector.x, vector.y, vector.z, 1.0f ) * conjugate;
	vector.x = result.x;
	vector.y = result.y;
	vector.z = result.z;
}
