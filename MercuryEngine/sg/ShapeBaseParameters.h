// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <sg/ShapeType.h>
#include <me/ITexture.h>
#include <me/Effect.h>
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
    	ShapeBaseParameters & SetDiffuse( unify::Color diffuse );
    	ShapeBaseParameters & SetSpecular( unify::Color specular );
        ShapeBaseParameters & SetCenter( unify::V3< float > center );
	    ShapeBaseParameters & SetTextureMode( int textureMode );
		ShapeBaseParameters & SetEffect( me::Effect::ptr effect );
    };
}
