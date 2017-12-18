// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <me/render/IPixelShader.h>
#include <rm/ResourceManagerSimple.h>
#include <me/game/IGame.h>

namespace me
{
	class PixelShaderFactory : public rm::ISourceFactory< render::IPixelShader >
	{
	public:
		PixelShaderFactory( game::IGame * gameInstance );
		render::IPixelShader::ptr Produce( unify::Path source, void * data ) override;

	public:
		game::IGame * m_game;
	};
}