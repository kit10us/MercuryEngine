// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/RenderInfo.h>
#include <dxi/core/IRenderer.h>
#include <dxi/VertexDeclaration.h>
#include <unify/Path.h>

namespace dxi
{
	class VertexShader
	{
	public:
		typedef std::shared_ptr< VertexShader > ptr;

		static void DisuseShader();

		VertexShader( core::IRenderer * renderer );

		VertexShader( core::IRenderer * renderer, const unify::Path & filePath, const std::string & entryPointName, const std::string & profile, VertexDeclaration::ptr vertexDeclaration );

		~VertexShader();

		void Destroy();

		void Create();

		void CreateFromFile( const unify::Path & filePath, const std::string & entryPointName, const std::string & profile );

		void CreateFromCode( const std::string & code, const std::string & entryPointName, const std::string & profile );

		void SetVertexDeclaration( VertexDeclaration::ptr vertexDeclaration );

		VertexDeclaration::ptr GetVertexDeclaration() const;

		const void * GetBytecode() const;

		size_t GetBytecodeLength() const;

		void Use( const RenderInfo & renderInfo );

		std::string GetError();

	protected:	   
		unify::Path m_filePath;
		std::string m_code;
		bool m_assembly;
		std::string m_entryPointName;
		std::string m_profile;
		std::string m_errorMessage;
		bool m_created;
		VertexDeclaration::ptr m_vertexDeclaration;

		class Pimpl;
		std::shared_ptr< Pimpl > m_pimpl;
	};
}