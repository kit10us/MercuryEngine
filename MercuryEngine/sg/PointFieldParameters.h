// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
#pragma once

#include <sg/ShapeBaseParameters.h>

namespace sg
{
    struct PointFieldParameters : public ShapeBaseParameters
    {
        PointFieldParameters();
		PointFieldParameters( const ShapeBaseParameters & parameters );

        unify::Parameters & Reset();
        PointFieldParameters & SetMajorRadius( const float radius );
        PointFieldParameters & SetMinorRadius( const float radius );
        PointFieldParameters & SetCount( unsigned int count );
    };
}