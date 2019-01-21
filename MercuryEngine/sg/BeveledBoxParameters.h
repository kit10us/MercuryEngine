// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <sg/ShapeBaseParameters.h>
#include <unify/BBox.h>
#include <unify/Matrix.h>

namespace sg
{
    struct BeveledBoxParameters : public ShapeBaseParameters
    {
        BeveledBoxParameters();
		BeveledBoxParameters( const ShapeBaseParameters & parameters );

        unify::Parameters & Reset();
        BeveledBoxParameters & SetBBox( const unify::BBox< float > & bbox, unify::Matrix & transformation = unify::MatrixIdentity() );
        BeveledBoxParameters & SetInf( const unify::V3< float > & inf, unify::Matrix & transformation = unify::MatrixIdentity() );
        BeveledBoxParameters & SetSup( const unify::V3< float > & sup, unify::Matrix & transformation = unify::MatrixIdentity() );
        BeveledBoxParameters & SetSize( const unify::Size3< float > & size );
        BeveledBoxParameters & SetDiffuseFaces( unify::Color one, unify::Color two, unify::Color three, unify::Color four, unify::Color five, unify::Color six );
        BeveledBoxParameters & SetDiffusePoints( unify::Color one, unify::Color two, unify::Color three, unify::Color four, unify::Color five, unify::Color six, unify::Color seven, unify::Color eight );
        BeveledBoxParameters & SetDefintion( size_t definition );
        BeveledBoxParameters & SetBevel( unify::Size3< float > & bevel );
    };
}