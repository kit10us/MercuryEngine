// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <medx11/Renderer.h>
#include <me/shader/ConstantBuffer.h>
#include <me/VertexDeclaration.h>
#include <unify/Path.h>
#include <atlbase.h>

namespace medx11
{
	class VertexShader : public me::IVertexShader
	{
	public:
		VertexShader( me::IRenderer * renderer );
		VertexShader( me::IRenderer * renderer, me::VertexShaderParameters parameters );

		~VertexShader();

		void Destroy();

		void Create( me::VertexShaderParameters parameters );

		const me::shader::ConstantBuffer * GetConstants() const override;

		void LockConstants( size_t buffer, unify::DataLock & lock ) override;	   

		void UnlockConstants( size_t buffer, unify::DataLock & lock ) override;

		void SetVertexDeclaration( me::VertexDeclaration::ptr vertexDeclaration );

		me::VertexDeclaration::ptr GetVertexDeclaration() const override;

		const void * GetBytecode() const override;

		size_t GetBytecodeLength() const override;

		void Use() override;

		unify::Path GetSource() const override;

		bool Reload() override;

		bool IsTrans() const override;

		std::string GetError();

	protected:	   
		me::VertexShaderParameters m_parameters;
		bool m_assembly;
		std::string m_errorMessage;
		bool m_created;
		
		me::shader::ConstantBuffer::ptr m_constants;

		me::VertexDeclaration::ptr m_vertexDeclaration;
		Renderer * m_renderer;

		CComPtr< ID3D11VertexShader > m_vertexShader;
		CComPtr< ID3D10Blob > m_vertexShaderBuffer;
		//CComPtr< ID3D11ShaderReflection > m_vertexShaderReflection;

		std::vector< ID3D11Buffer * > m_constantBuffers;
		size_t m_bufferAccessed;
		size_t m_locked;
	};
}