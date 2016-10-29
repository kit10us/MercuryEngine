// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <unify/Quaternion.h>
#include <unify/Matrix.h>


/*
#include <DirectXMath.h>
#define XMV( _a ) *(DirectX::XMVECTOR*)&_a
*/

using namespace unify;



Quaternion unify::QuaternionIdentity()
{
	Quaternion q( 0, 0, 0, 1 );
	return q;
}

Quaternion unify::QuaternionFromEuler( const unify::V3< float > & euler )
{
	Quaternion q;

	float c1 = cos( euler.y );
	float c2 = cos( euler.x );
	float c3 = cos( euler.z );
	float s1 = sin( euler.y );
	float s2 = sin( euler.x );
	float s3 = sin( euler.z );

	q.w = sqrt( 1.0f + c1 * c2 + c1*c3 - s1 * s2 * s3 + c2*c3 ) / 2.0f;
	q.x = (c2 * s3 + c1 * s3 + s1 * s2 * c3) / (4.0f * q.w);
	q.y = (s1 * c2 + s1 * c3 + c1 * s2 * s3) / (4.0f * q.w);
	q.z = (-s1 * s3 + c1 * s2 * c3 + s2) / (4.0f * q.w);

	return q;
}

Quaternion unify::QuaternionSlerp( const Quaternion & qa, const Quaternion & qb, float t )
{
	Quaternion q;
	
	float cosHalfTheta = qa.w * qb.w + qa.x * qb.x + qa.y * qb.y + qa.z * qb.z;
	if ( abs( cosHalfTheta ) >= 1.0f )
	{
		q.w = qa.w; q.x = qa.x; q.y = qa.y; q.z = qa.z;
		return q;
	}

	float halfTheta = acos( cosHalfTheta );
	float sinHalfTheta = sqrt( 1.0f - cosHalfTheta*cosHalfTheta );

	if ( fabs( sinHalfTheta ) < 0.001f ) 
	{ // fabs is floating point absolute
		q.w = (qa.w * 0.5f + qb.w * 0.5f);
		q.x = (qa.x * 0.5f + qb.x * 0.5f);
		q.y = (qa.y * 0.5f + qb.y * 0.5f);
		q.z = (qa.z * 0.5f + qb.z * 0.5f);
		return q;
	}
	float ratioA = sin( (1.0f - t) * halfTheta ) / sinHalfTheta;
	float ratioB = sin( t * halfTheta ) / sinHalfTheta;

	q.w = (qa.w * ratioA + qb.w * ratioB);
	q.x = (qa.x * ratioA + qb.x * ratioB);
	q.y = (qa.y * ratioA + qb.y * ratioB);
	q.z = (qa.z * ratioA + qb.z * ratioB);

	return q;
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

Quaternion::Quaternion( V3< float > axis, Angle angle )
{
	float s = sin( angle.GetRadians() / 2.0f );
	x = axis.x * s;
	y = axis.y * s;
	z = axis.z * s;
	w = cos( angle.GetRadians() / 2.0f );
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
	return x == 0.0f && y == 0.0f && z == 0.0f && w == 1.0f;
}

Quaternion Quaternion::Conjugate() const
{
	return Quaternion( -x, -y, -z, w );
}

Quaternion Quaternion::Normalize() const
{	
	float n = sqrt( x * x + y * y + z * z + w * w );
	return Quaternion( x / n, y / n, z / n, w / n );
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
	Quaternion o;
	const Quaternion & a = *this;
	const Quaternion & b = quaternion;
	o.x = b.w * a.x + b.x * a.w + b.y * a.z - b.z * a.y;
	o.y = b.w * a.y - b.x * a.z + b.y * a.w + b.z * a.x;
	o.z = b.w * a.z + b.x * a.y - b.y * a.x + b.z * a.w;
	o.w = b.w * a.w - b.x * a.x - b.y * a.y - b.z * a.z;
	*this = o;

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

bool Quaternion::operator == ( const Quaternion & quaternion ) const
{
	return (x == quaternion.x && y == quaternion.y && z == quaternion.z && w == quaternion.w) ? true : false;
}

bool Quaternion::operator != ( const Quaternion & quaternion ) const
{
	return !operator==( quaternion );
}

V3< float > Quaternion::operator * ( V3< float > v ) const
{
	TransformVector( v );
	return v;
}
 
void Quaternion::TransformVector( V3< float > & v ) const
{
	Quaternion q( v.x, v.y, v.z, 1 );
	q *= *this;
	v.x = q.x;
	v.y = q.y;
	v.z = q.z;



	/*
	float length = v.Normalize();
	Quaternion conjugate( Conjugate() );
	Quaternion result = *this * Quaternion( v.x, v.y, v.z, 1.0f ) * conjugate;
	v.x = result.x;
	v.y = result.y;
	v.z = result.z;
	*/



	/*
	void rotate_vector_by_quaternion( const Vector3& v, const Quaternion& q, Vector3& vprime )
	{
		// Extract the vector part of the quaternion
		Vector3 u( q.x, q.y, q.z );

		// Extract the scalar part of the quaternion
		float s = q.w;

		// Do the math
		vprime = 2.0f * dot( u, v ) * u
			+ (s*s - dot( u, u )) * v
			+ 2.0f * s * cross( u, v );
	}
	*/
	
	/*

	V3< float > u( x, y, z );
	V3< float > out;

	// Extract the scalar part of the quaternion
	float s = w;

	// Do the math
	v = 2.0f * u.Dot( v ) * u
		+ (s*s - u.Dot u ) * v
		+ 2.0f * s * u.Cross( v );
	}
	*/
}

std::string Quaternion::ToString() const
{
	return Cast< std::string >( x ) + ", " + Cast< std::string >( y ) + ", " + Cast< std::string >( z ) + ", " + Cast< std::string >( w );
}

