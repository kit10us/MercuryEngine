// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <medx9/Renderer.h>
#include <atlbase.h>
#include <unify/Path.h>
#include <memory>

namespace medx9
{
	class PixelShader : public me::IPixelShader
	{
	public:
		PixelShader( me::IRenderer * renderer );
		PixelShader( me::IRenderer * renderer, me::PixelShaderParameters parameters );

		~PixelShader();

		void Destroy();

		void Create( me::PixelShaderParameters parameters );

		const me::shader::ConstantBuffer * GetConstants() const override;

		void LockConstants( size_t buffer, unify::DataLock & lock ) override;	   

		void UnlockConstants( size_t buffer, unify::DataLock & lock ) override;

		void Use() override;

		std::string GetSource() const override;

		void SetTrans( bool bTrans ) override;

		bool IsTrans() override;

		std::string GetError();

	protected:
		me::PixelShaderParameters m_parameters;
		bool m_assembly;
		std::string m_errorMessage;
		bool m_created;
 		me::shader::ConstantBuffer::ptr m_constants;

		bool m_isTrans;	// Does this pixel shader turn the render into transparent (in part or entire)

		const Renderer * m_renderer;
		CComPtr< ID3DXBuffer > m_codeBuffer;
		CComPtr< ID3DXConstantTable > m_constantTable;
		CComPtr< IDirect3DPixelShader9 > m_shader;
	};
}