// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <dxi/Texture.h>
#include <rm/ResourceManagerSimple.h>

namespace dxi
{
	class TextureSourceFactory : public rm::ISourceFactory< Texture >
	{
	public:
		Texture * Produce( unify::Path source ) override;
	};
}