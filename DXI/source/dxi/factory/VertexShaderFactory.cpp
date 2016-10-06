// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/factory/VertexShaderFactory.h>
#include <dxi/exception/FailedToCreate.h>
#include <qxml/Document.h>

using namespace dxi;

VertexShader * VertexShaderFactory::Produce( unify::Path source )
{
	qxml::Document doc( source );
	auto & node = *doc.GetRoot()->FindFirstElement( "vertexshader" );
	unify::Path shaderSource = node.GetDocument()->GetPath().DirectoryOnly() + node.GetElement( "source" )->GetText();
	std::string entry = node.GetElement( "entry" )->GetText();
	std::string profile = node.GetElement( "profile" )->GetText();
	VertexDeclaration::ptr vertexDeclaration( new VertexDeclaration( *node.GetElement( "vd" ) ) );
	return new VertexShader( shaderSource, entry, profile, vertexDeclaration );
}
