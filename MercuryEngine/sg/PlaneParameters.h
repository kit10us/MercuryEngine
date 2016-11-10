// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <sg/ShapeBaseParameters.h>
#include <unify/TexArea.h>

namespace sg
{
    struct PlaneParameters : public ShapeBaseParameters
    {
        PlaneParameters();
        unify::Parameters & Reset();
	    PlaneParameters & SetSize( unify::Size< float > size );
	    PlaneParameters & SetSegments( unsigned int segments );
        PlaneParameters & SetTexArea( unify::TexArea texArea );
    };
}