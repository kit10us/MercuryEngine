// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <sg/ShapeBaseParameters.h>

using namespace sg;
using namespace me;
using namespace render;

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

ShapeBaseParameters & ShapeBaseParameters::SetEffect( Effect::ptr effect )
{
    Set< Effect::ptr >( "effect", effect );
	return *this;
}

ShapeBaseParameters & ShapeBaseParameters::SetFaceUV( std::vector< unify::TexArea > faceUV )
{
	Set< std::vector< unify::TexArea > >( "faceUV", faceUV );
	return *this;
}
