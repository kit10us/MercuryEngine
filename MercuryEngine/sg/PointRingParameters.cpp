// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <sg/PointRingParameters.h>

using namespace sg;

PointRingParameters::PointRingParameters()
: ShapeBaseParameters( Shape::PointRing )
{
}

PointRingParameters::PointRingParameters( const ShapeBaseParameters & parameters )
	: ShapeBaseParameters( parameters, Shape::PointRing )
{
}

unify::Parameters & PointRingParameters::Reset()
{
    Reset();
    Set< Shape::TYPE >( "type", Shape::PointRing );
    return *this;
}

PointRingParameters & PointRingParameters::SetMajorRadius( const float radius )
{
    Set< float >( "majorradius", radius );
    return *this;
}

PointRingParameters & PointRingParameters::SetMinorRadius( const float radius )
{
    Set< float >( "minorradius", radius );
    return *this;
}

PointRingParameters & PointRingParameters::SetCount( unsigned int count )
{
    Set< unsigned int >( "count", count );
    return *this;
}