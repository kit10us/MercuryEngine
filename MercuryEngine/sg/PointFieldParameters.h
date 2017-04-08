// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <sg/ShapeBaseParameters.h>

namespace sg
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