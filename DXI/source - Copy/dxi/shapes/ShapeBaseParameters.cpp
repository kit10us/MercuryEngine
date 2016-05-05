// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/shapes/ShapeBaseParameters.h>

using namespace dxi;
using namespace shapes;

ShapeBaseParameters::ShapeBaseParameters()
{
}

ShapeBaseParameters::ShapeBaseParameters( Shape::TYPE type )
{
    Set< Shape::TYPE >( "type", type );
}

ShapeBaseParameters & ShapeBaseParameters::SetDiffuse( unify::Color diffuse )
{
    Set< unify::Color>( "diffuse", diffuse );
	return *this;
}

ShapeBaseParameters & ShapeBaseParameters::SetSpecular( unify::Color specular )
{
    Set< unify::Color>( "specular", specular );
	return *this;
}

ShapeBaseParameters & ShapeBaseParameters::SetCenter( unify::V3< float > center )
{
    Set< unify::V3< float > >( "center", center );
	return *this;
}

ShapeBaseParameters & ShapeBaseParameters::SetTextureMode( int textureMode )
{
    Set< int >( "texturemode", textureMode );
	return *this;
}

ShapeBaseParameters & ShapeBaseParameters::SetEffect( Effect::shared_ptr effect )
{
    Set< Effect::shared_ptr >( "effect", effect );
	return *this;
}
