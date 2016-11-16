// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/RenderInfo.h>
#include <me/IRenderer.h>
#include <me/IVertexShader.h>
#include <me/VertexDeclaration.h>
#include <unify/Path.h>

namespace medx9
{
	class VertexShader : public me::IVertexShader
	{
	public:
		VertexShader( const me::IRenderer * renderer );
		VertexShader( const me::IRenderer * renderer, me::VertexShaderParameters parameters );

		~VertexShader();

		void Destroy();

		void Create( me::VertexShaderParameters parameters );

		void SetVertexDeclaration( me::VertexDeclaration::ptr vertexDeclaration );

		me::VertexDeclaration::ptr GetVertexDeclaration() const override;

		const void * GetBytecode() const;

		size_t GetBytecodeLength() const;

		void Use( const me::RenderInfo & renderInfo, const me::RenderInstance & instance );

		std::string GetError();

	protected:	   
		unify::Path m_filePath;
		std::string m_code;
		bool m_assembly;
		std::string m_entryPointName;
		std::string m_profile;
		std::string m_errorMessage;
		bool m_created;
		me::VertexDeclaration::ptr m_vertexDeclaration;

		class Pimpl;
		std::shared_ptr< Pimpl > m_pimpl;
	};
}