// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>
#include <unify/Matrix.h>

namespace unify
{
	class FrameLite
	{
	public:
		FrameLite();
		virtual ~FrameLite();

		void SetMatrix( const Matrix & matrix );

		void LookAt( const FrameLite & at, const V3< float > & up = V3< float >( 0, 1, 0 ) );
		void LookAt( const V3< float > & at, const V3< float > & up = V3< float >( 0, 1, 0 ) );
		void LookDirection( const V3< float > & direction, const V3< float > & left );
		void MoveBy( const V3< float > & by );
		void Slide( const V3< float > & axis, float amount );
		void Orbit( const V3< float > & origin, const V2< float > & direction, Angle angle );
		void Orbit( const V3< float > & origin, const Quaternion & orbit, float distance );
		void Rotate( const V3< float > & axis, Angle angle );
		void Rotate( const Quaternion & q );
		void RotateAbout( const V3< float > & axis, Angle angle );

		const Matrix & GetMatrix() const;

		V3< float > GetLeft() const;
		V3< float > GetUp() const;
		V3< float > GetForward() const;
		Quaternion GetRotation() const;
		V3< float > GetPosition() const;

		void SetLeft( const V3< float > & left );
		void SetUp( const V3< float > & up );
		void SetForward( const V3< float > & direction );
		void SetRotation( const Quaternion & rotation );
		void SetPosition( const V3< float > & position );
		void Set( const Quaternion & orientation, const V3< float > & position, const V3< float > & scale = V3< float >( 1, 1, 1 ) );

	protected:
		Matrix m_matrix;
	};
}