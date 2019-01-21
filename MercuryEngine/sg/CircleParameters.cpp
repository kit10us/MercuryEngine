// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <sg/CircleParameters.h>

using namespace sg;

CircleParameters::CircleParameters()
: ShapeBaseParameters( Shape::Circle )
{
}

CircleParameters::CircleParameters( const ShapeBaseParameters & parameters )
	: ShapeBaseParameters( parameters, Shape::Circle )
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
