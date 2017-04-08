// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/render/Mesh.h>
#include <rm/ResourceManagerSimple.h>
#include <me/Game.h>

namespace ase
{
	class GeometryFactory : public rm::ISourceFactory< me::render::Geometry >
	{
	public:
		GeometryFactory( me::Game * game );

		void SetVertexShader( me::render::IVertexShader::ptr vertexShader );
		void SetPixelShader( me::render::IPixelShader::ptr pixelShader );

		me::render::Geometry::ptr Produce( unify::Path source, void * data ) override;

	private:
		me::Game * m_game;
		// TODO: This will likely need to have multiple sets based on material needs - TBD.
		me::render::IVertexShader::ptr m_vertexShader;
		me::render::IPixelShader::ptr m_pixelShader;
	};
}