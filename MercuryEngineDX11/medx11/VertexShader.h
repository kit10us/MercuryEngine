// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <medx11/Renderer.h>
#include <me/RenderInfo.h>
#include <me/IVertexShader.h>
#include <me/shader/ShaderConstants.h>
#include <me/VertexDeclaration.h>
#include <unify/Path.h>
#include <atlbase.h>

namespace medx11
{
	class VertexShader : public me::IVertexShader
	{
	public:
		VertexShader( const me::IRenderer * renderer );
		VertexShader( const me::IRenderer * renderer, me::VertexShaderParameters parameters );

		~VertexShader();

		void Destroy();

		void Create( me::VertexShaderParameters parameters );

		const me::shader::ShaderConstants * GetConstants() const override;

		void LockConstants( size_t buffer, unify::DataLock & lock ) override;	   

		void UnlockConstants( size_t buffer, unify::DataLock & lock ) override;

		void SetVertexDeclaration( me::VertexDeclaration::ptr vertexDeclaration );

		me::VertexDeclaration::ptr GetVertexDeclaration() const override;

		const void * GetBytecode() const override;

		size_t GetBytecodeLength() const override;

		void Use() override;

		std::string GetError();

	protected:	   
		unify::Path m_filePath;
		std::string m_code;
		bool m_assembly;
		std::string m_entryPointName;
		std::string m_profile;
		std::string m_errorMessage;
		bool m_created;
		
		me::shader::ShaderConstants::ptr m_constants;

		me::VertexDeclaration::ptr m_vertexDeclaration;
		const Renderer * m_renderer;

		CComPtr< ID3D11VertexShader > m_vertexShader;
		CComPtr< ID3D10Blob > m_vertexShaderBuffer;
		//CComPtr< ID3D11ShaderReflection > m_vertexShaderReflection;

		std::vector< ID3D11Buffer * > m_constantBuffers;
		size_t m_bufferAccessed;
		size_t m_locked;
	};
}