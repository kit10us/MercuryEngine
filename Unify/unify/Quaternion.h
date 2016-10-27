// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>
#include <unify/V3.h>
#include <unify/V4.h>
#include <unify/Angle.h>

namespace unify
{
	class Quaternion
	{
		friend Quaternion QuaternionIdentity();
		friend Quaternion QuaternionFromEuler( const unify::V3< float > & euler );
		friend Quaternion QuaternionSlerp( const Quaternion & quaternionA, const Quaternion & quaternionB, float mix );
	public:

		Quaternion();
		Quaternion( V3< float > axis, Angle angle );
		Quaternion( float x, float y, float z, float w );
		Quaternion( const Quaternion & quaternion );

		bool IsIdentity() const;

		Quaternion Conjugate() const;
		Quaternion Normalize() const;

		// assignment operators
		Quaternion & operator += ( const Quaternion & quaternion );
		Quaternion & operator -= ( const Quaternion & quaternion );
		Quaternion & operator *= ( const Quaternion & quaternion );
		Quaternion & operator *= ( float value );

		// binary operators
		Quaternion operator + ( const Quaternion & quaternion ) const;
		Quaternion operator - ( const Quaternion & quaternion ) const;
		Quaternion operator * ( const Quaternion & quaternion ) const;
		Quaternion operator * ( float value ) const;

		bool operator == ( const Quaternion & quaternion ) const;
		bool operator != ( const Quaternion & quaternion ) const;

		V3< float > operator * ( V3< float > quaternion ) const;

		void TransformVector( V3< float > & vector ) const;

		float x;
		float y;
		float z;
		float w;
	};

	Quaternion QuaternionIdentity();
	Quaternion QuaternionFromEuler( const unify::V3< float > & euler );
	Quaternion QuaternionSlerp( const Quaternion & quaternionA, const Quaternion & quaternionB, float mix );
}
