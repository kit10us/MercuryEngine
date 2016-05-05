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
        struct CubeParameters : public ShapeBaseParameters
        {
            CubeParameters();
            unify::Parameters & Reset();
            CubeParameters & SetBBox( const unify::BBox< float > & bbox, unify::Matrix & transformation = unify::Matrix::MatrixIdentity() );
            CubeParameters & SetInf( const unify::V3< float > & inf, unify::Matrix & transformation = unify::Matrix::MatrixIdentity() );
            CubeParameters & SetSup( const unify::V3< float > & sup, unify::Matrix & transformation = unify::Matrix::MatrixIdentity() );
            CubeParameters & SetSize( const unify::Size3< float > & size );
            CubeParameters & SetDiffuseFaces( unify::Color one, unify::Color two, unify::Color three, unify::Color four, unify::Color five, unify::Color six );
            CubeParameters & SetDiffusePoints( unify::Color one, unify::Color two, unify::Color three, unify::Color four, unify::Color five, unify::Color six, unify::Color seven, unify::Color eight );
        };
    }
}