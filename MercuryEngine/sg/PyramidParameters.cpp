// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <sg/PyramidParameters.h>

using namespace sg;

PyramidParameters::PyramidParameters()
: ShapeBaseParameters( Shape::Pyramid )
{
}

unify::Parameters & PyramidParameters::Reset()
{
    Reset();
    Set< Shape::TYPE >( "type", Shape::Pyramid );
    return *this;
}

PyramidParameters & PyramidParameters::SetSize( unify::Size3< float > size )
{
    Set< unify::Size3< float > >( "size3", size );
    return *this;
}

PyramidParameters & PyramidParameters::SetDiffusePoints( unify::Color colorOne, unify::Color colorTwo, unify::Color colorThree, unify::Color colorFour, unify::Color colorFive )
{
    std::vector< unify::Color > diffuses;
    diffuses.push_back( colorOne );
    diffuses.push_back( colorTwo );
    diffuses.push_back( colorThree );
    diffuses.push_back( colorFour );
    diffuses.push_back( colorFive );
    Set< std::vector< unify::Color > >( "diffuses", diffuses );
    return *this;
}
