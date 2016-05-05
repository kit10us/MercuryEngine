// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/shapes/SphereParameters.h>

using namespace dxi;
using namespace shapes;

SphereParameters::SphereParameters()
: ShapeBaseParameters( Shape::Sphere )
{
}

unify::Parameters & SphereParameters::Reset()
{
    Reset();
    Set< Shape::TYPE >( "type", Shape::Sphere );
    return *this;
}

SphereParameters & SphereParameters::SetRadius( float radius )
{
    Set< float >( "radius", radius );
    return *this;
}

SphereParameters & SphereParameters::SetSegments( unsigned int segments )
{
    Set< unsigned int >( "segments", segments );
    return *this;
}
