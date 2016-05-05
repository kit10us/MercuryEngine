// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/PixelShaderFactory.h>

using namespace dxi;

PixelShader * PixelShaderFactory::Produce( const qjson::Object & object )
{
	return new PixelShader( object["path"]->ToString(), object["entry"]->ToString(), object["profile"]->ToString() );
}