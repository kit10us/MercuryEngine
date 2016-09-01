// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/PixelShaderFactories.h>
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

PixelShader * PixelShaderXMLFactory::Produce( unify::Path path )
{
	qxml::Document doc( path );
	qxml::Element * element = doc.FindElement( "pixelshader" );
	return Produce( element );
}

PixelShader * PixelShaderXMLFactory::Produce( const qxml::Element * node )
{
	unify::Path path { node->GetDocument()->GetPath().DirectoryOnly() + node->GetAttribute( "source" )->GetString() };
	std::string entry = node->GetAttribute( "entry" )->GetString();
	std::string profile = node->GetAttribute( "profile" )->GetString();
	return new PixelShader( path, entry, profile );
}
