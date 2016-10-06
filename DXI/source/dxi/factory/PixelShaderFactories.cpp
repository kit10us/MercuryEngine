// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/factory/PixelShaderFactories.h>
#include <dxi/exception/FailedToCreate.h>
#include <qxml/Document.h>

using namespace dxi;

PixelShader * PixelShaderXMLFactory::Produce( const qxml::Element & node )
{
	unify::Path path { node.GetDocument()->GetPath().DirectoryOnly() + node.GetElement( "source" )->GetText() };
	std::string entry = node.GetElement( "entry" )->GetText();
	std::string profile = node.GetElement( "profile" )->GetText();
	return new PixelShader( path, entry, profile );
}
