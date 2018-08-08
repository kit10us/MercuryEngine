// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <sg/TubeParameters.h>

using namespace sg;

TubeParameters::TubeParameters()
: ShapeBaseParameters( Shape::Tube )
{
}

unify::Parameters & TubeParameters::Reset()
{
    Reset();
    Set< Shape::TYPE >( "type", Shape::Tube );
    return *this;
}

TubeParameters & TubeParameters::SetMajorRadius( float outer )
{
    Set< float >( "majorradius", outer );
    return *this;
}

TubeParameters & TubeParameters::SetMinorRadius( float inner )
{
    Set< float >( "minorradius", inner );
    return *this;
}

TubeParameters & TubeParameters::SetSegments( unsigned int segments )
{
    Set< unsigned int >( "segments", segments );
    return *this;
}

TubeParameters & TubeParameters::SetHeight( float height )
{
    Set< float >( "height", height );
    return *this;
}