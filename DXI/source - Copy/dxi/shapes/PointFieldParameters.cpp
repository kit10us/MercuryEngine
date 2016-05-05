// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/shapes/PointFieldParameters.h>

using namespace dxi;
using namespace shapes;

PointFieldParameters::PointFieldParameters()
: ShapeBaseParameters( Shape::PointField )
{
}

unify::Parameters & PointFieldParameters::Reset()
{
    Reset();
    Set< Shape::TYPE >( "type", Shape::PointField );
    return *this;
}

PointFieldParameters & PointFieldParameters::SetMajorRadius( const float radius )
{
    Set< float >( "majorradius", radius );
    return *this;
}

PointFieldParameters & PointFieldParameters::SetMinorRadius( const float radius )
{
    Set< float >( "minorradius", radius );
    return *this;
}

PointFieldParameters & PointFieldParameters::SetCount( unsigned int count )
{
    Set< unsigned int >( "count", count );
    return *this;
}
