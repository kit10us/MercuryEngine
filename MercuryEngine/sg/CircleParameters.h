// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
#pragma once

#include <sg/ShapeBaseParameters.h>

namespace sg
{
    struct CircleParameters : public ShapeBaseParameters
    {
        CircleParameters();
		CircleParameters( const ShapeBaseParameters & parameters );

        unify::Parameters & Reset();
	    CircleParameters & SetSegments( unsigned int segments );
	    CircleParameters & SetRadius( float radius );
    };
}