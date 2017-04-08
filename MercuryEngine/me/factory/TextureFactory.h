// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <me/render/ITexture.h>
#include <me/IGame.h>
#include <rm/ResourceManagerSimple.h>

namespace me
{
	class TextureSourceFactory : public rm::ISourceFactory< render::ITexture >
	{
	public:
		TextureSourceFactory( IGame * game );
		render::ITexture::ptr Produce( unify::Path source, void * data ) override;

	private:
		IGame * m_game;
	};
}