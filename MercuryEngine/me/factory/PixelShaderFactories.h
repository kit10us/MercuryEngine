// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
#pragma once
#include <me/render/IPixelShader.h>
#include <rm/ResourceManager.h>
#include <me/game/IGame.h>

namespace me
{
	class PixelShaderFactory : public rm::ISourceFactory< render::IPixelShader >
	{
	public:
		typedef std::shared_ptr< rm::ISourceFactory< me::render::IPixelShader > > ptr;

		PixelShaderFactory( game::IGame * gameInstance );
		render::IPixelShader::ptr Produce( unify::Path source, void * data ) override;

	public:
		game::IGame * m_game;
	};
}