// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <me/IPixelShader.h>
#include <rm/ResourceManagerSimple.h>
#include <me/IGame.h>

namespace me
{
	class PixelShaderFactory : public rm::ISourceFactory< IPixelShader >
	{
	public:
		PixelShaderFactory( IGame * game );
		IPixelShader::ptr Produce( unify::Path source, void * data ) override;

	public:
		IGame * m_game;
	};
}