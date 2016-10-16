// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <dxi/Texture.h>
#include <rm/ResourceManagerSimple.h>
#include <dxi/core/Game.h>

namespace dxi
{
	class TextureSourceFactory : public rm::ISourceFactory< Texture >
	{
	public:
		TextureSourceFactory( core::Game * game );
		Texture::ptr Produce( unify::Path source ) override;

	private:
		core::Game * m_game;
	};
}