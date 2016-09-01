// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <dxi/VertexShader.h>
#include <rm/ResourceManagerSimple.h>

namespace dxi
{
	class VertexShaderJsonFactory : public rm::IJsonFactory< VertexShader >
	{
	public:
		VertexShader * Produce( qjson::Object json ) override;
	};

	qjson::Object MakeVertexShaderJson( std::string name, unify::Path path, std::string entry, std::string profile, qjson::Object vertexDeclaration );


	class VertexShaderXMLFactory : public rm::IXMLFactory< VertexShader >
	{
	public:
		VertexShader * Produce( unify::Path path ) override;
		VertexShader * Produce( const qxml::Element * node ) override;
	};
}