// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>
#include <unify/V3.h>
#include <unify/V4.h>

namespace unify
{
	class Matrix;

	class Quaternion
	{
	public:
		static Quaternion QuaternionIdentity();
		static Quaternion QuaternionFromEuler( const unify::V3< float > & euler );
		static Quaternion QuaternionRotationAxis( const V3< float > & axis, float rotation );
		static Quaternion QuaternionSlerp( const Quaternion & quaternionA, const Quaternion & quaternionB, float mix );

		Quaternion();
		Quaternion( float x, float y, float z, float w );
		Quaternion( const Quaternion & quaternion );
		Quaternion::Quaternion( const Matrix & matrix );

		bool IsIdentity() const;

		// assignment operators
		Quaternion & operator += ( const Quaternion & quaternion );
		Quaternion & operator -= ( const Quaternion & quaternion );
		Quaternion & operator *= ( const Quaternion & quaternion );
		Quaternion & operator *= ( float value );
		Quaternion & operator /= ( float value );

		// unary operators
		Quaternion operator + () const;
		Quaternion operator - () const;

		// binary operators
		Quaternion operator + ( const Quaternion & quaternion ) const;
		Quaternion operator - ( const Quaternion & quaternion ) const;
		Quaternion operator * ( const Quaternion & quaternion ) const;
		Quaternion operator * ( float value ) const;
		Quaternion operator / ( float value ) const;

		bool operator == ( const Quaternion & quaternion ) const;
		bool operator != ( const Quaternion & quaternion ) const;
		
		Quaternion Conjugate() const;

		void TransformVector( V3< float > & vector ) const;

		float x;
		float y;
		float z;
		float w;
	};
}
