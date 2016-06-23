// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/ShaderBase.h>
#include <dxi/VertexDeclaration.h>
#include <qjson/Object.h>

namespace dxi
{
	// TODO: Could it be possible to make this EVENMORE generic, so we can use it for all kinds of shaders, including Geometry, etc.?
	// At least enough to support having ShaderBase in the ResourceManager?
	class VertexShader : public ShaderBase
	{
	public:
		typedef std::shared_ptr< VertexShader > shared_ptr;

		static void DisuseShader();

		VertexShader();
		VertexShader( const unify::Path & filePath, const std::string & entryPointName, const std::string & profile, VertexDeclaration vertexDeclaration );

		void SetVertexDeclaration( VertexDeclaration vertexDeclaration );
		VertexDeclaration GetVertexDeclaration() const;

		// ::ShaderBase::QResource...
		void Destroy();

		// ::ShaderBase...
		void Use( const RenderInfo * renderInfo );

	protected:
		void CreateThisShader();

		IDirect3DVertexShader9 * m_shader;
		VertexDeclaration m_vertexDeclaration;
		D3DXHANDLE m_finalMatrixHandle; // Combination Model * View * Projection matrix location in shader.
	};
}