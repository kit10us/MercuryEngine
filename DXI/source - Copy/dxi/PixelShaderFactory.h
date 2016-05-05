// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/PixelShader.h>
#include <dxi/ResourceManager.h>

namespace dxi
{
	class PixelShaderFactory : public dxi::ResourceManager< PixelShader >::IFactory
	{
	public:
		PixelShader* Produce( const qjson::Object & object );
	};
}