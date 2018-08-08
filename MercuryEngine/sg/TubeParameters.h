// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <sg/ShapeBaseParameters.h>

namespace sg
{
    struct TubeParameters : public ShapeBaseParameters
    {
        TubeParameters();
        unify::Parameters & Reset();
	    TubeParameters & SetMajorRadius( float radius );
	    TubeParameters & SetMinorRadius( float radius );
	    TubeParameters & SetSegments( unsigned int segments );
	    TubeParameters & SetHeight( float height );
    };
}