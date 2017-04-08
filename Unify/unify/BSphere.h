/* Copyright (c) 2002 - 2018, Quentin S. Smith
 * All Rights Reserved
 */

#pragma once

#include <unify/Unify.h>
#include <unify/V2.h>
#include <unify/V3.h>
#include <unify/V4.h>

namespace unify
{
	// An accumilated radius (from origin).
	class BSphere
	{
		float radius;
	public:
		BSphere( float radius );
		void AddBSphere( const BSphere & sphere );
		void AddBSphereWithOffset( const BSphere & sphere, float offsetAsLength );
		void AddPoint( V2< float > & point );
		void AddPoint( V3< float > & point );
		void AddPoint( V4< float > & point );
	};
	#include <unify/BSphere.inl>
}


