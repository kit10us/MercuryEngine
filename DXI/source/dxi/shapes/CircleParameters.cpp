// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/shapes/CircleParameters.h>

using namespace dxi;
using namespace shapes;

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
