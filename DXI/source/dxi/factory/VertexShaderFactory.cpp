// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/factory/VertexShaderFactory.h>
#include <dxi/exception/FailedToCreate.h>
#include <qxml/Document.h>

using namespace dxi;

VertexShader * VertexShaderXMLFactory::Produce( const qxml::Element & node )
{
	unify::Path source = node.GetDocument()->GetPath().DirectoryOnly() + node.GetElement( "source" )->GetText();
	std::string entry = node.GetElement( "entry" )->GetText();
	std::string profile = node.GetElement( "profile" )->GetText();
	VertexDeclaration::ptr vertexDeclaration( new VertexDeclaration( *node.GetElement( "vd" ) ) );
	return new VertexShader( source, entry, profile, vertexDeclaration );
}
