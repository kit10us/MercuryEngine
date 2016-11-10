// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <sg/CircleParameters.h>

using namespace sg;

CircleParameters::CircleParameters()
: ShapeBaseParameters( Shape::Circle )
{
}

unify::Parameters & CircleParameters::Reset()
{
    Reset();
    Set< Shape::TYPE >( "type", Shape::Circle );
    return *this;
}

CircleParameters & CircleParameters::SetSegments( unsigned int segments )
{
    Set< unsigned int >( "segments", segments );
    return *this;
}

CircleParameters & CircleParameters::SetRadius( float radius )
{
    Set< float >( "radius", radius );
    return *this;
}
