// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <me/IVertexShader.h>
#include <rm/ResourceManagerSimple.h>
#include <me/IGame.h>

namespace me
{
	class VertexShaderFactory : public rm::ISourceFactory< IVertexShader >
	{
	public:
		VertexShaderFactory( IGame * game );
		IVertexShader::ptr Produce( unify::Path source, void * data ) override;

	private:
		IGame * m_game;
	};
}