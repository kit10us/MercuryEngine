// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/VertexShaderFactory.h>
#include <dxi/exception/FailedToCreate.h>

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

qjson::Object dxi::MakeVertexShaderJson( unify::Path path, std::string entry, std::string profile, qjson::Object vertexDeclaration )
{
	return { { "source", path.ToString() },{ "entry", entry },{ "profile", profile }, { "vd", vertexDeclaration } };
}

VertexShader * VertexShaderXMLFactory::Produce( const qxml::Element * node )
{
	unify::Path path = node->GetAttribute( "source" )->GetString();
	std::string entry = node->GetAttribute( "entry" )->GetString();
	std::string profile = node->GetAttribute( "profile" )->GetString();
	VertexDeclaration vertexDeclaration = VertexDeclaration( *node->GetElement( "vd" ) );
	return new VertexShader( path, entry, profile, vertexDeclaration );
}