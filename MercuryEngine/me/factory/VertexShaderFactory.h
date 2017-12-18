// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/render/IVertexShader.h>
#include <me/game/IGame.h>
#include <rm/ResourceManagerSimple.h>

namespace me
{
	class VertexShaderFactory : public rm::ISourceFactory< render::IVertexShader >
	{
	public:
		VertexShaderFactory( game::IGame * gameInstance );
	render::	IVertexShader::ptr Produce( unify::Path source, void * data ) override;

	private:
		game::IGame * m_game;
	};
}