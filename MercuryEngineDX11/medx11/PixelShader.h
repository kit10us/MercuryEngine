// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <medx11/Renderer.h>
#include <unify/Path.h>
#include <memory>
#include <atlbase.h>

namespace medx11
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

		unify::Path GetSource() const override;

		bool Reload() override;

		bool IsTrans() const;

		std::string GetError();

	protected:
		me::PixelShaderParameters m_parameters;
		std::string m_errorMessage;
		bool m_created;

		bool m_isTrans;	// Does this pixel shader turn the render into transparent (in part or entire)

		Renderer * m_renderer;
		CComPtr< ID3D11PixelShader > m_pixelShader;
		CComPtr< ID3D10Blob > m_pixelShaderBuffer;

		me::shader::ConstantBuffer::ptr m_constants;
		std::vector< ID3D11Buffer * > m_constantBuffers;
		size_t m_bufferAccessed;
		size_t m_locked;

		CComPtr< ID3D11BlendState > m_blendState;
		D3D11_BLEND_DESC m_blendDesc;
	};
}