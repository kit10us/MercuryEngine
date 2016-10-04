// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <dxi/Mesh.h>
#include <rm/ResourceManagerSimple.h>

namespace dxi
{
	class GeometryASEFactory : public rm::ISourceFactory< Geometry >
	{
	public:
		void SetVertexShader( dxi::VertexShader::ptr vertexShader );
		void SetPixelShader( dxi::PixelShader::ptr pixelShader );

		Geometry * Produce( unify::Path source ) override;

	private:
		// TODO: This will likely need to have multiple sets based on material needs - TBD.
		dxi::VertexShader::ptr m_vertexShader;
		dxi::PixelShader::ptr m_pixelShader;
	};
}