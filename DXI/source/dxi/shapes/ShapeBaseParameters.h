// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/shapes/ShapeType.h>
#include <dxi/Texture.h>
#include <dxi/Effect.h>
#include <unify/Parameters.h>
#include <unify/Size3.h>
#include <unify/V3.h>

namespace dxi
{
    namespace shapes
    {
        struct ShapeBaseParameters : public unify::Parameters
        {
            ShapeBaseParameters();
            ShapeBaseParameters( Shape::TYPE type );
    	    ShapeBaseParameters & SetDiffuse( unify::Color diffuse );
    	    ShapeBaseParameters & SetSpecular( unify::Color specular );
            ShapeBaseParameters & SetCenter( unify::V3< float > center );
	        ShapeBaseParameters & SetTextureMode( int textureMode );
			ShapeBaseParameters & SetEffect( Effect::shared_ptr effect );
        };
    }
}
