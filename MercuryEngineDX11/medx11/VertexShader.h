// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <medx11/Renderer.h>
#include <me/RenderInfo.h>
#include <me/IVertexShader.h>
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

		void SetVertexDeclaration( me::VertexDeclaration::ptr vertexDeclaration );

		me::VertexDeclaration::ptr GetVertexDeclaration() const override;

		const void * GetBytecode() const;

		size_t GetBytecodeLength() const;

		void Use( const me::RenderInfo & renderInfo, const unify::Matrix & world );

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
		const Renderer * m_renderer;

		struct ConstantBuffer
		{
			unify::Matrix worldMatrix;
			unify::Matrix viewMatrix;
			unify::Matrix projectionMatrix;
		} m_vertexShaderConstants;

		CComPtr< ID3D11VertexShader > m_vertexShader;
		CComPtr< ID3D10Blob > m_vertexShaderBuffer;
		//CComPtr< ID3D11ShaderReflection > m_vertexShaderReflection;

		// TODO: Need a standard interface for updating constant data that is cross renderer support.
		CComPtr< ID3D11Buffer > m_vertexShaderConstantBuffer;
	};
}