// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <sg/ShapeBaseParameters.h>
#include <unify/TexArea.h>

namespace sg
{
    struct CylinderParameters : public ShapeBaseParameters
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