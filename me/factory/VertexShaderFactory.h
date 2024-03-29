// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/render/IVertexShader.h>
#include <me/game/IGame.h>
#include <rm/ResourceManager.h>

namespace me
{
	class VertexShaderFactory : public rm::ISourceFactory< render::IVertexShader >
	{
	public:
		typedef std::shared_ptr< rm::ISourceFactory< me::render::IVertexShader > > ptr;

		VertexShaderFactory( game::IGame * gameInstance );
		render::IVertexShader::ptr Produce( unify::Path source, unify::Parameters parameters ) override;
		render::IVertexShader::ptr Produce( unify::Parameters parameters ) override;

	private:
		game::IGame * m_game;
	};
}