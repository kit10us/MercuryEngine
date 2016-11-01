// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/shapes/ShapeBaseParameters.h>
#include <unify/BBox.h>
#include <unify/Matrix.h>

namespace dxi
{
    namespace shapes
    {
        struct BeveledBoxParameters : public ShapeBaseParameters
        {
            BeveledBoxParameters();
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
}