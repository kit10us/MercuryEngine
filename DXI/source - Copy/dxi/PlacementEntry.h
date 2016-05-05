// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Matrix.h>

namespace dxi
{
	struct PlacementEntry
	{
		unsigned int uGeometryID;	// Later (for now, just allow one geometry)
		unify::V3< float > vPos;				// When by position
		unify::Matrix	mMat;			// When by matrix

		PlacementEntry( const unify::V3< float > & position )
			: vPos( position )
		{
		}
	};
} // namespace dxi