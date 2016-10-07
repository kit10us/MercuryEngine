// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <dxi/PixelShader.h>
#include <rm/ResourceManagerSimple.h>
#include <dxi/core/Game.h>

namespace dxi
{
	class PixelShaderFactory : public rm::ISourceFactory< PixelShader >
	{
	public:
		PixelShaderFactory( core::Game * game );
		PixelShader * Produce( unify::Path source ) override;

	public:
		core::Game * m_game;
	};
}