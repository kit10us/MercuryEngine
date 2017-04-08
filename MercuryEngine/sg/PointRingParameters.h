// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <sg/ShapeBaseParameters.h>

namespace sg
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