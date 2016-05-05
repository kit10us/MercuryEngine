// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/shapes/ShapeBaseParameters.h>

namespace dxi
{
    namespace shapes
    {
        struct CylinderParameters : public shapes::ShapeBaseParameters
        {
            CylinderParameters();
            unify::Parameters & Reset();
	        CylinderParameters & SetRadius( float radius );
	        CylinderParameters & SetSegments( unsigned int segments );
	        CylinderParameters & SetHeight( float height );
            CylinderParameters & SetTexArea( unify::TexArea texArea );
	        CylinderParameters & SetCaps( bool caps );
        };
    }
}