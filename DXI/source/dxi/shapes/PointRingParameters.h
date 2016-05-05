// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/shapes/ShapeBaseParameters.h>

namespace dxi
{
    namespace shapes
    {
        struct PointRingParameters : public ShapeBaseParameters
        {
            PointRingParameters();
            unify::Parameters & Reset();
            PointRingParameters & SetMajorRadius( const float radius );
            PointRingParameters & SetMinorRadius( const float radius );
            PointRingParameters & SetCount( unsigned int count );
        };
    }
}