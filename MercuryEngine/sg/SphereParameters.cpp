// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <sg/SphereParameters.h>

using namespace sg;

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
