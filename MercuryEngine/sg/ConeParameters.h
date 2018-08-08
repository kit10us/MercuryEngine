// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <sg/ShapeBaseParameters.h>

namespace sg
{
    struct ConeParameters : public ShapeBaseParameters
    {
        ConeParameters();
        unify::Parameters & Reset();
	    ConeParameters & SetSegments( unsigned int segments );
	    ConeParameters & SetRadius( float radius );
	    ConeParameters & SetHeight( float height );
	    ConeParameters & SetCaps( bool caps );
    };
}