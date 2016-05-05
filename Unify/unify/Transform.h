// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>
#include <unify/V3.h>
#include <unify/Quaternion.h>
#include <unify/Matrix.h>

namespace unify
{
	class Transform
	{
	public:
		Transform();
		Transform( const Transform & transform );
		explicit Transform( const Matrix & matrix );
		explicit Transform( const Quaternion quaternion, const V3< float > position = V3< float >( 0, 0, 0 ), const V3< float > scale = V3< float >( 1, 1, 1 ) );
		~Transform();

		const V3< float > & GetPosition() const;
		const V3< float > & GetScale() const;
		const Quaternion & GetOrientation() const;
		Matrix MakeMatrix() const;

	protected:
		V3< float > m_position;
		V3< float > m_scale;
		Quaternion m_orientation;
	};
}