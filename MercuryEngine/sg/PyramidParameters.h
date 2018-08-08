// Copyright (c) 2002 - 2018, Evil Quail LLC
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