// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <me/Mesh.h>
#include <rm/ResourceManagerSimple.h>
#include <me/Game.h>

namespace ase
{
	class GeometryFactory : public rm::ISourceFactory< me::Geometry >
	{
	public:
		GeometryFactory( me::Game * game );

		void SetVertexShader( me::IVertexShader::ptr vertexShader );
		void SetPixelShader( me::IPixelShader::ptr pixelShader );

		me::Geometry::ptr Produce( unify::Path source, void * data ) override;

	private:
		me::Game * m_game;
		// TODO: This will likely need to have multiple sets based on material needs - TBD.
		me::IVertexShader::ptr m_vertexShader;
		me::IPixelShader::ptr m_pixelShader;
	};
}