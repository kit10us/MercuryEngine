// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
#pragma once

#include <sg/ShapeBaseParameters.h>

namespace sg
{
    struct PointRingParameters : public ShapeBaseParameters
    {
        PointRingParameters();
		PointRingParameters( const ShapeBaseParameters & parameters );

        unify::Parameters & Reset();
        PointRingParameters & SetMajorRadius( const float radius );
        PointRingParameters & SetMinorRadius( const float radius );
        PointRingParameters & SetCount( unsigned int count );
    };
}