// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/render/IVertexShader.h>
#include <me/IGame.h>
#include <rm/ResourceManagerSimple.h>

namespace me
{
	class VertexShaderFactory : public rm::ISourceFactory< render::IVertexShader >
	{
	public:
		VertexShaderFactory( IGame * game );
	render::	IVertexShader::ptr Produce( unify::Path source, void * data ) override;

	private:
		IGame * m_game;
	};
}