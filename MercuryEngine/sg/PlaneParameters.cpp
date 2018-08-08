// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <sg/PlaneParameters.h>

using namespace sg;

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