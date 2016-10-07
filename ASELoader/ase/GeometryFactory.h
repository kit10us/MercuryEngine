// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <dxi/Mesh.h>
#include <rm/ResourceManagerSimple.h>
#include <dxi/core/Game.h>

namespace ase
{
	class GeometryFactory : public rm::ISourceFactory< dxi::Geometry >
	{
	public:
		GeometryFactory( dxi::core::Game * game );

		void SetVertexShader( dxi::VertexShader::ptr vertexShader );
		void SetPixelShader( dxi::PixelShader::ptr pixelShader );

		dxi::Geometry * Produce( unify::Path source ) override;

	private:
		dxi::core::Game * m_game;
		// TODO: This will likely need to have multiple sets based on material needs - TBD.
		dxi::VertexShader::ptr m_vertexShader;
		dxi::PixelShader::ptr m_pixelShader;
	};
}