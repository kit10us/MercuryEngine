// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <me/render/IPixelShader.h>
#include <rm/ResourceManagerSimple.h>
#include <me/IGame.h>

namespace me
{
	class PixelShaderFactory : public rm::ISourceFactory< render::IPixelShader >
	{
	public:
		PixelShaderFactory( IGame * game );
		render::IPixelShader::ptr Produce( unify::Path source, void * data ) override;

	public:
		IGame * m_game;
	};
}