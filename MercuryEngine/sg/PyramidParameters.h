// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <sg/ShapeBaseParameters.h>

namespace sg
{
    struct PyramidParameters : public ShapeBaseParameters
    {
        PyramidParameters();
        unify::Parameters & Reset();
        PyramidParameters & SetSize( unify::Size3< float > size );
        PyramidParameters & SetDiffusePoints( unify::Color colorOne, unify::Color colorTwo, unify::Color colorThree, unify::Color colorFour, unify::Color colorFive );
    };
}