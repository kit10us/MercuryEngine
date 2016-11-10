// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <sg/ConeParameters.h>

using namespace sg;

ConeParameters::ConeParameters()
: ShapeBaseParameters( Shape::Cone )
{
}

unify::Parameters & ConeParameters::Reset()
{
    Reset();
    Set< Shape::TYPE >( "type", Shape::Cone );
    return *this;
}

ConeParameters & ConeParameters::SetSegments( unsigned int segments )
{
    Set< unsigned int >( "segments", segments );
    return *this;
}

ConeParameters & ConeParameters::SetRadius( float radius )
{
    Set< float >( "radius", radius );
    return *this;
}

ConeParameters & ConeParameters::SetHeight( float height )
{
    Set< float >( "height", height );
    return *this;
}

ConeParameters & ConeParameters::SetCaps( bool caps )
{
    Set< bool >( "caps", caps );
    return *this;
}
