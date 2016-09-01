// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/VertexShaderFactory.h>
#include <dxi/exception/FailedToCreate.h>
#include <qxml/Document.h>

using namespace dxi;

VertexShader * VertexShaderJsonFactory::Produce( qjson::Object json )
{
	unify::Path path = json[ "source" ].ToString();
	std::string entry = json["entry"].ToString();
	std::string profile = json["profile"].ToString();
	qjson::Object vd = { json["vd"] };
	VertexDeclaration vertexDeclaration = VertexDeclaration( vd );
	return new VertexShader( path, entry, profile, vertexDeclaration );
}

qjson::Object dxi::MakeVertexShaderJson( std::string name, unify::Path path, std::string entry, std::string profile, qjson::Object vertexDeclaration )
{
	return { { "name", name }, { "source", path.ToString() },{ "entry", entry },{ "profile", profile }, { "vd", vertexDeclaration } };
}

VertexShader * VertexShaderXMLFactory::Produce( unify::Path path )
{
	qxml::Document doc( path );
	qxml::Element * element = doc.FindElement( "vertexshader" );
	return Produce( element );
}

VertexShader * VertexShaderXMLFactory::Produce( const qxml::Element * node )
{
	//std::string name = node.GetAttribute< std::string >( "name" );
	unify::Path source = node->GetDocument()->GetPath().DirectoryOnly() + node->GetAttribute< std::string >( "source" );
	std::string entry = node->GetAttribute( "entry" )->GetString();
	std::string profile = node->GetAttribute( "profile" )->GetString();
	VertexDeclaration vertexDeclaration = VertexDeclaration( *node->GetElement( "vd" ) );
	return new VertexShader( source, entry, profile, vertexDeclaration );
}
