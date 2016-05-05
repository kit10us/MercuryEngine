// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/VertexShader.h>
#include <dxi/ResourceManager.h>

namespace dxi
{
	class VertexShaderFactory : public ResourceManager< VertexShader >::IFactory
	{
	public:
		VertexShader * Produce( const qjson::Object & object );
	};

	typedef std::shared_ptr< ResourceManager< VertexShader > > VertexShaderManager;
}