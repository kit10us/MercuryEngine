// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <sg/ShapeBaseParameters.h>

namespace sg
{
    struct SphereParameters : public ShapeBaseParameters
    {
        SphereParameters();
		SphereParameters( const ShapeBaseParameters & parameters );

        unify::Parameters & Reset();
	    SphereParameters & SetRadius( float radius );
	    SphereParameters & SetSegments( unsigned int segments );
    };
}