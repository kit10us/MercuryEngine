// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <dxi/PixelShader.h>
#include <rm/ResourceManagerSimple.h>

namespace dxi
{
	class PixelShaderJsonFactory : public rm::IJsonFactory< PixelShader >
	{
	public:
		PixelShader * Produce( qjson::Object json ) override;
	};

	qjson::Object MakePixelShaderJson( std::string name, unify::Path path, std::string entry, std::string profile );

	class PixelShaderXMLFactory : public rm::IXMLFactory< PixelShader >
	{
	public:
		PixelShader * Produce( const qxml::Element & node ) override;
	};
}