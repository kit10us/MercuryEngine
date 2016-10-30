// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <dxi/Texture.h>
#include <rm/ResourceManagerSimple.h>
#include <dxi/core/IGame.h>

namespace dxi
{
	class TextureSourceFactory : public rm::ISourceFactory< Texture >
	{
	public:
		TextureSourceFactory( core::IGame * game );
		Texture::ptr Produce( unify::Path source, void * data ) override;

	private:
		core::IGame * m_game;
	};
}