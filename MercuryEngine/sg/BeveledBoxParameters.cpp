// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <sg/BeveledBoxParameters.h>

using namespace sg;

BeveledBoxParameters::BeveledBoxParameters()
: ShapeBaseParameters( Shape::BeveledBox )
{
}

unify::Parameters & BeveledBoxParameters::Reset()
{
    Reset();
    Set< Shape::TYPE >( "type", Shape::BeveledBox );
    return *this;
}

BeveledBoxParameters & BeveledBoxParameters::SetBBox( const unify::BBox< float > & bbox, unify::Matrix & transformation )
{
    Set<>( "inf", transformation.TransformCoord( bbox.inf ) );
    Set<>( "sup", transformation.TransformCoord( bbox.sup ) );
    return *this;
} 

BeveledBoxParameters & BeveledBoxParameters::SetInf( const unify::V3< float > & inf, unify::Matrix & transformation )
{
    Set<>( "inf", inf );
    return *this;
}

BeveledBoxParameters & BeveledBoxParameters::SetSup( const unify::V3< float > & sup, unify::Matrix & transformation )
{
    Set< unify::V3< float > >( "sup", sup );
    return *this;
}

BeveledBoxParameters & BeveledBoxParameters::SetSize( const unify::Size3< float > & size )
{
    Set< unify::Size3< float > >( "size3", size );
    return *this;
}

BeveledBoxParameters & BeveledBoxParameters::SetDiffuseFaces( unify::Color one, unify::Color two, unify::Color three, unify::Color four, unify::Color five, unify::Color six )
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

BeveledBoxParameters & BeveledBoxParameters::SetDiffusePoints( unify::Color one, unify::Color two, unify::Color three, unify::Color four, unify::Color five, unify::Color six, unify::Color seven, unify::Color eight )
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

BeveledBoxParameters & BeveledBoxParameters::SetDefintion( size_t definition )
{
    Set< size_t >( "defintion", definition );
    return *this;
}

BeveledBoxParameters & BeveledBoxParameters::SetBevel( unify::Size3< float > & bevel )
{
    Set< unify::Size3< float > >( "bevel", bevel );
    return *this;
}
