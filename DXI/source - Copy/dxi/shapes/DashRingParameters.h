// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/shapes/ShapeBaseParameters.h>

namespace dxi
{
    namespace shapes
    {
        struct DashRingParameters : public shapes::ShapeBaseParameters
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
}