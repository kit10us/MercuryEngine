// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <dxi/VertexShader.h>
#include <rm/ResourceManagerSimple.h>
#include <dxi/core/IGame.h>

namespace dxi
{
	class VertexShaderFactory : public rm::ISourceFactory< VertexShader >
	{
	public:
		VertexShaderFactory( core::IGame * game );
		VertexShader::ptr Produce( unify::Path source, void * data ) override;

	private:
		core::IGame * m_game;
	};
}