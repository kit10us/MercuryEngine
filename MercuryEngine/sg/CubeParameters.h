// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
#pragma once

#include <sg/ShapeBaseParameters.h>
#include <unify/BBox.h>
#include <unify/Matrix.h>

namespace sg
{
    struct CubeParameters : public ShapeBaseParameters
    {
        CubeParameters();
		CubeParameters( const ShapeBaseParameters & parameters );

        unify::Parameters & Reset();
        CubeParameters & SetBBox( const unify::BBox< float > & bbox, unify::Matrix & transformation = unify::MatrixIdentity() );
        CubeParameters & SetInf( const unify::V3< float > & inf, unify::Matrix & transformation = unify::MatrixIdentity() );
        CubeParameters & SetSup( const unify::V3< float > & sup, unify::Matrix & transformation = unify::MatrixIdentity() );
        CubeParameters & SetSize( const unify::Size3< float > & size );
        CubeParameters & SetDiffuseFaces( unify::Color one, unify::Color two, unify::Color three, unify::Color four, unify::Color five, unify::Color six );
        CubeParameters & SetDiffusePoints( unify::Color one, unify::Color two, unify::Color three, unify::Color four, unify::Color five, unify::Color six, unify::Color seven, unify::Color eight );
    };
}