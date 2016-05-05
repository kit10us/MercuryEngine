// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/shapes/PlaneParameters.h>

using namespace dxi;
using namespace shapes;

PlaneParameters::PlaneParameters()
: ShapeBaseParameters( Shape::Plane )
{
}

unify::Parameters & PlaneParameters::Reset()
{
    Reset();
    Set< Shape::TYPE >( "type", Shape::Plane );
    return *this;
}

PlaneParameters & PlaneParameters::SetSize( unify::Size< float > size )
{
    Set< unify::Size< float > >( "size2", size );
    return *this;
}

PlaneParameters & PlaneParameters::SetSegments( unsigned int segments )
{
    Set< unsigned int >( "segments", segments );
    return *this;
}

PlaneParameters & PlaneParameters::SetTexArea( unify::TexArea texArea )
{
    Set< unify::TexArea >( "texarea", texArea );
    return *this;
}