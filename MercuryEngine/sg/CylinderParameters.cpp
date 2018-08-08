// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <sg/CylinderParameters.h>

using namespace sg;

CylinderParameters::CylinderParameters()
: ShapeBaseParameters( Shape::Cylinder )
{
}

unify::Parameters & CylinderParameters::Reset()
{
    Reset();
    Set< Shape::TYPE >( "type", Shape::Cylinder );
    return *this;
}

CylinderParameters & CylinderParameters::SetRadius( float radius )
{
    Set< float >( "radius", radius );
    return *this;
}

CylinderParameters & CylinderParameters::SetSegments( unsigned int segments )
{
    Set< unsigned int >( "segments", segments );
    return *this;
}

CylinderParameters & CylinderParameters::SetHeight( float height )
{
    Set< float >( "height", height );
    return *this;
}

CylinderParameters & CylinderParameters::SetTexArea( unify::TexArea texArea )
{
    Set< unify::TexArea >( "texarea", texArea );
    return *this;
}

CylinderParameters & CylinderParameters::SetCaps( bool caps )
{
    Set< bool >( "caps", caps );
    return *this;
}
