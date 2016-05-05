// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/shapes/ShapeBaseParameters.h>

namespace dxi
{
    namespace shapes
    {
        struct PointFieldParameters : public ShapeBaseParameters
        {
            PointFieldParameters();
            unify::Parameters & Reset();
            PointFieldParameters & SetMajorRadius( const float radius );
            PointFieldParameters & SetMinorRadius( const float radius );
            PointFieldParameters & SetCount( unsigned int count );
        };
    }
}