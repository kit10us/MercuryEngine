// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/RenderInfo.h>
#include <unify/Path.h>
#include <dxi/VertexDeclaration.h>

namespace dxi
{
	class VertexShader
	{
	public:
		typedef std::shared_ptr< VertexShader > shared_ptr;

		static void DisuseShader();

		VertexShader();
		
		VertexShader( const unify::Path & filePath, const std::string & entryPointName, const std::string & profile, VertexDeclaration vertexDeclaration );
		
		~VertexShader();

		void Destroy();

		void Create();

		void CreateFromFile( const unify::Path & filePath, const std::string & entryPointName, const std::string & profile );

		void CreateFromCode( const std::string & code, const std::string & entryPointName, const std::string & profile );

		void SetVertexDeclaration( VertexDeclaration vertexDeclaration );

		VertexDeclaration GetVertexDeclaration() const;

		void Use( const RenderInfo & renderInfo );

		std::string GetError();

		ID3DXConstantTable * GetConstantTable();

	protected:

		unify::Path m_filePath;
		std::string m_code;
		bool m_assembly;
		std::string m_entryPointName;
		std::string m_profile;
		std::string m_errorMessage;
		bool m_created;
		VertexDeclaration m_vertexDeclaration;

		class Pimpl;
		std::shared_ptr< Pimpl > m_pimpl;
	};
}