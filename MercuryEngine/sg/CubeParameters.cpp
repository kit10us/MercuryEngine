// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <sg/CubeParameters.h>

using namespace sg;

CubeParameters::CubeParameters()
: ShapeBaseParameters( Shape::Cube )
{
}

CubeParameters::CubeParameters( const ShapeBaseParameters & parameters )
	: ShapeBaseParameters( parameters, Shape::Cube )
{
}

unify::Parameters & CubeParameters::Reset()
{
    Reset();
    Set< Shape::TYPE >( "type", Shape::Cube );
    return *this;
}

CubeParameters & CubeParameters::SetBBox( const unify::BBox< float > & bbox, unify::Matrix & transformation )
{
    Set< unify::V3< float > >( "inf", transformation.TransformCoord( bbox.inf ) );
    Set< unify::V3< float > >( "sup", transformation.TransformCoord( bbox.sup ) );
    return *this;
} 

CubeParameters & CubeParameters::SetInf( const unify::V3< float > & inf, unify::Matrix & transformation )
{
    Set< unify::V3< float > >( "inf", inf );
    return *this;
}

CubeParameters & CubeParameters::SetSup( const unify::V3< float > & sup, unify::Matrix & transformation )
{
    Set< unify::V3< float > >( "sup", sup );
    return *this;
}

CubeParameters & CubeParameters::SetSize( const unify::Size3< float > & size )
{
    Set< unify::Size3< float > >( "size3", size );
    return *this;
}

CubeParameters & CubeParameters::SetDiffuseFaces( unify::Color one, unify::Color two, unify::Color three, unify::Color four, unify::Color five, unify::Color six )
{
    std::vector< unify::Color > diffuses;
    diffuses.push_back( one );
    diffuses.push_back( two );
    diffuses.push_back( three );
    diffuses.push_back( four );
    diffuses.push_back( five );
    diffuses.push_back( six );
    Set< std::vector< unify::Color > >( "diffuses", diffuses );
    return *this;
}     

CubeParameters & CubeParameters::SetDiffusePoints( unify::Color one, unify::Color two, unify::Color three, unify::Color four, unify::Color five, unify::Color six, unify::Color seven, unify::Color eight )
{
    std::vector< unify::Color > diffuses;
    diffuses.push_back( one );
    diffuses.push_back( two );
    diffuses.push_back( three );
    diffuses.push_back( four );
    diffuses.push_back( five );
    diffuses.push_back( six );
    diffuses.push_back( seven );
    diffuses.push_back( eight );
    Set< std::vector< unify::Color > >( "diffuses", diffuses );
    return *this;
}      
