// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once
#include <me/render/ITexture.h>
#include <me/game/IGame.h>
#include <rm/ResourceManagerSimple.h>

namespace me
{
	class TextureSourceFactory : public rm::ISourceFactory< render::ITexture >
	{
	public:
		TextureSourceFactory( game::IGame * gameInstance );
		render::ITexture::ptr Produce( unify::Path source, void * data ) override;

	private:
		game::IGame * m_game;
	};
}