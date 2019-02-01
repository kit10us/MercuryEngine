// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
#pragma once

#include <sg/ShapeBaseParameters.h>
#include <unify/TexArea.h>

namespace sg
{
    struct PlaneParameters : public ShapeBaseParameters
    {
        PlaneParameters();
		PlaneParameters( const ShapeBaseParameters & parameters );

        unify::Parameters & Reset();
	    PlaneParameters & SetSize( unify::Size< float > size );
	    PlaneParameters & SetSegments( unsigned int segments );
        PlaneParameters & SetTexArea( unify::TexArea texArea );
    };
}