// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/shapes/ShapeBaseParameters.h>

namespace dxi
{
    namespace shapes
    {
        struct ConeParameters : public shapes::ShapeBaseParameters
        {
            ConeParameters();
            unify::Parameters & Reset();
	        ConeParameters & SetSegments( unsigned int segments );
	        ConeParameters & SetRadius( float radius );
	        ConeParameters & SetHeight( float height );
	        ConeParameters & SetCaps( bool caps );
        };
    }
}