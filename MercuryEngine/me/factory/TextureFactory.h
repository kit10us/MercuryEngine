// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <me/ITexture.h>
#include <me/IGame.h>
#include <rm/ResourceManagerSimple.h>

namespace me
{
	class TextureSourceFactory : public rm::ISourceFactory< ITexture >
	{
	public:
		TextureSourceFactory( IGame * game );
		ITexture::ptr Produce( unify::Path source, void * data ) override;

	private:
		IGame * m_game;
	};
}