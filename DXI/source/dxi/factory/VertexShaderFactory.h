// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <dxi/VertexShader.h>
#include <rm/ResourceManagerSimple.h>

namespace dxi
{
	class VertexShaderXMLFactory : public rm::IXMLFactory< VertexShader >
	{
	public:
		VertexShader * Produce( const qxml::Element & node ) override;
	};
}