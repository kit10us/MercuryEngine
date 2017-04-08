// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <sg/DashRingParameters.h>

using namespace sg;

DashRingParameters::DashRingParameters()
: ShapeBaseParameters( Shape::DashRing )
{
}

unify::Parameters & DashRingParameters::Reset()
{
    Reset();
    Set< Shape::TYPE >( "type", Shape::DashRing );
    return *this;
}

DashRingParameters & DashRingParameters::SetMajorRadius( float outer )
{
    Set< float >( "majorradius", outer );
    return *this;
}

DashRingParameters & DashRingParameters::SetMinorRadius( float inner )
{
    Set< float >( "minorradius", inner );
    return *this;
}

DashRingParameters & DashRingParameters::SetCount( unsigned int count )
{
    Set< unsigned int >( "count", count );
    return *this;
}

DashRingParameters & DashRingParameters::SetSize( float size )
{
    Set< float >( "size1", size );
    return *this;
}

DashRingParameters & DashRingParameters::SetDefinition( float definition )
{
    Set< float >( "definition", definition );
    return *this;
}
