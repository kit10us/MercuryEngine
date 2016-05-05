// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>
#include <unify/V3.h>

namespace unify
{
	template< typename T >
	class Ray
	{
	public:
		V3< T > origin;
		V3< T > direction;

		V3< T > InvDirection() const;
		V3< int > Sign() const;

        Ray();
		Ray( const V3< T > & originIn, const V3< T > & directionIn );
	};
	#include <unify/Ray.inl>
}