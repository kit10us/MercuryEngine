// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/RenderInfo.h>
#include <medx9/Renderer.h>
#include <me/IVertexShader.h>
#include <me/VertexDeclaration.h>
#include <unify/Path.h>
#include <atlbase.h>

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

		const me::shader::ConstantBuffer * GetConstants() const override;

		void LockConstants( size_t buffer, unify::DataLock & lock ) override;	  
		void UnlockConstants( size_t buffer, unify::DataLock & lock ) override;	  

		void SetVertexDeclaration( me::VertexDeclaration::ptr vertexDeclaration ); 
		me::VertexDeclaration::ptr GetVertexDeclaration() const override;

		const void * GetBytecode() const override;

		size_t GetBytecodeLength() const override;

		void Use() override;

		std::string GetSource() const override;

		std::string GetError();

	protected:	   
		unify::Path m_filePath;
		std::string m_code;
		bool m_assembly;
		std::string m_entryPointName;
		std::string m_profile;
		std::string m_errorMessage;
		bool m_created;
		me::shader::ConstantBuffer::ptr m_constants;
		me::VertexDeclaration::ptr m_vertexDeclaration;

		const medx9::Renderer * m_renderer;

		CComPtr< ID3DXBuffer > m_codeBuffer;
		CComPtr< ID3DXConstantTable > m_constantTable;
		CComPtr< IDirect3DVertexShader9 > m_shader;

		std::vector< unify::Matrix > m_lockData;
	
		D3DXHANDLE m_worldMatrixHandle;
		D3DXHANDLE m_viewMatrixHandle;
		D3DXHANDLE m_projectionMatrixHandle;
	};
}