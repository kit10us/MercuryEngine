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
		FrameLite( unify::Quaternion q, unify::V3< float > p );
		virtual ~FrameLite();

		void LookAt( const FrameLite & at, const V3< float > & up = V3< float >( 0, 1, 0 ) );
		void LookAt( const V3< float > & at, const V3< float > & up = V3< float >( 0, 1, 0 ) );
		void MoveBy( const V3< float > & by );
		void Orbit( const V3< float > & origin, const V2< float > & direction, Angle angle );
		void Orbit( const V3< float > & origin, const Quaternion & orbit );
		void PreMul( Quaternion q );
		void PostMul( Quaternion q );

		Matrix GetMatrix() const;

		V3< float > GetLeft() const;
		V3< float > GetUp() const;
		V3< float > GetForward() const;
		Quaternion GetRotation() const;
		V3< float > GetPosition() const;

		void SetRotation( const Quaternion & rotation );
		void SetPosition( const V3< float > & position );

		void SetModelMatrix( Matrix & modelMatrix );
		const Matrix & GetModelMatrix() const;
		bool GetUseModelMatrix() const;

	protected:
		unify::Quaternion m_q;
		unify::V3< float > m_p;
		
		bool m_useModelMatrix;
		unify::Matrix m_modelMatrix;
	};
}