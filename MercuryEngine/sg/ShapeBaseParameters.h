// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <sg/ShapeType.h>
#include <me/render/ITexture.h>
#include <me/render/Effect.h>
#include <unify/Parameters.h>
#include <unify/Size3.h>
#include <unify/V3.h>
#include <unify/Color.h>

namespace sg
{
    struct ShapeBaseParameters : public unify::Parameters
    {
        ShapeBaseParameters();
        ShapeBaseParameters( Shape::TYPE type );
		ShapeBaseParameters( const ShapeBaseParameters & parameters, Shape::TYPE type );

		ShapeBaseParameters & SetDiffuse( unify::Color diffuse );
    	ShapeBaseParameters & SetSpecular( unify::Color specular );
        ShapeBaseParameters & SetCenter( unify::V3< float > center );
	    ShapeBaseParameters & SetTextureMode( int textureMode );
		ShapeBaseParameters & SetEffect( me::render::Effect::ptr effect );
		ShapeBaseParameters & SetFaceUV( std::vector< unify::TexArea > faceUV );
		ShapeBaseParameters & SetFaceUV( size_t index, std::vector< unify::TexArea > faceUV );
	};
}
