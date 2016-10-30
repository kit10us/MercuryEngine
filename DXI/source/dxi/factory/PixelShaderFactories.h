// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <dxi/PixelShader.h>
#include <rm/ResourceManagerSimple.h>
#include <dxi/core/IGame.h>

namespace dxi
{
	class PixelShaderFactory : public rm::ISourceFactory< PixelShader >
	{
	public:
		PixelShaderFactory( core::IGame * game );
		PixelShader::ptr Produce( unify::Path source, void * data ) override;

	public:
		core::IGame * m_game;
	};
}