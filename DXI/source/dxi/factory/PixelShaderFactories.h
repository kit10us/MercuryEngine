// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <dxi/PixelShader.h>
#include <rm/ResourceManagerSimple.h>

namespace dxi
{
	class PixelShaderXMLFactory : public rm::IXMLFactory< PixelShader >
	{
	public:
		PixelShader * Produce( const qxml::Element & node ) override;
	};
}