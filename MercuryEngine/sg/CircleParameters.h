// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <sg/ShapeBaseParameters.h>

namespace sg
{
    struct CircleParameters : public ShapeBaseParameters
    {
        CircleParameters();
        unify::Parameters & Reset();
	    CircleParameters & SetSegments( unsigned int segments );
	    CircleParameters & SetRadius( float radius );
    };
}