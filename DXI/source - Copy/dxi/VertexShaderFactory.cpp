// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/VertexShaderFactory.h>

using namespace dxi;

VertexShader * VertexShaderFactory::Produce( const qjson::Object & object )
{
	return new VertexShader( object["path"]->ToString(), object["entry"]->ToString(), object["profile"]->ToString(), VertexDeclaration( qjson::Object( *object["vertex"].get() ) ) );
}
