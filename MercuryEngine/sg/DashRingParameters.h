// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <sg/ShapeBaseParameters.h>

namespace sg
{
    struct DashRingParameters : public ShapeBaseParameters
    {
        DashRingParameters();
        unify::Parameters & Reset();
        DashRingParameters & SetMajorRadius( float outer );
	    DashRingParameters & SetMinorRadius( float inner );
	    DashRingParameters & SetCount( unsigned int count );
	    DashRingParameters & SetSize( float size );
	    DashRingParameters & SetDefinition( float definition );
    };
}