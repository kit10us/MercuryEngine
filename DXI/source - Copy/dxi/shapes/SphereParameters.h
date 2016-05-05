// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/shapes/ShapeBaseParameters.h>

namespace dxi
{
    namespace shapes
    {
        struct SphereParameters : public shapes::ShapeBaseParameters
        {
            SphereParameters();
            unify::Parameters & Reset();
	        SphereParameters & SetRadius( float radius );
	        SphereParameters & SetSegments( unsigned int segments );
        };
    }
}