// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/shapes/ShapeBaseParameters.h>

namespace dxi
{
    namespace shapes
    {
        struct CircleParameters : public shapes::ShapeBaseParameters
        {
            CircleParameters();
            unify::Parameters & Reset();
	        CircleParameters & SetSegments( unsigned int segments );
	        CircleParameters & SetRadius( float radius );
        };
    }
}