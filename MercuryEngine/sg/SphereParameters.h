// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <sg/ShapeBaseParameters.h>

namespace sg
{
    struct SphereParameters : public ShapeBaseParameters
    {
        SphereParameters();
        unify::Parameters & Reset();
	    SphereParameters & SetRadius( float radius );
	    SphereParameters & SetSegments( unsigned int segments );
    };
}