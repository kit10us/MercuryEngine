// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <dxi/VertexShader.h>
#include <rm/ResourceManagerSimple.h>
#include <dxi/core/Game.h>

namespace dxi
{
	class VertexShaderFactory : public rm::ISourceFactory< VertexShader >
	{
	public:
		VertexShaderFactory( core::Game * game );
		VertexShader::ptr Produce( unify::Path source ) override;

	private:
		core::Game * m_game;
	};
}