// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/Texture.h>
#include <dxi/ResourceManager.h>

namespace dxi
{
	class TextureFactory : public ResourceManager< Texture >::IFactory
	{
	public:
		Texture * Produce( const qjson::Object & object );
	};
} 