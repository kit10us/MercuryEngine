// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/factory/PixelShaderFactories.h>
#include <dxi/exception/FailedToCreate.h>
#include <qxml/Document.h>

using namespace dxi;

PixelShader * PixelShaderJsonFactory::Produce( qjson::Object json )
{
	unify::Path path = json["source"].ToString();
	std::string entry = json["entry"].ToString();
	std::string profile = json["profile"].ToString();
	return new PixelShader( path, entry, profile );
}

qjson::Object dxi::MakePixelShaderJson( std::string name, unify::Path path, std::string entry, std::string profile )
{
	return { { "name", name }, { "source", path.ToString()}, { "entry", entry}, { "profile" , profile } };
}

PixelShader * PixelShaderXMLFactory::Produce( const qxml::Element & node )
{
	unify::Path path { node.GetDocument()->GetPath().DirectoryOnly() + node.GetElement( "source" )->GetText() };
	std::string entry = node.GetElement( "entry" )->GetText();
	std::string profile = node.GetElement( "profile" )->GetText();
	return new PixelShader( path, entry, profile );
}
