// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IRenderer.h>
#include <medx11/Renderer.h>
#include <unify/Path.h>
#include <memory>
#include <atlbase.h>

namespace medx11
{
	class PixelShader : public me::IPixelShader
	{
	public:
		PixelShader( const me::IRenderer * renderer );
		PixelShader( const me::IRenderer * renderer, me::PixelShaderParameters parameters );

		~PixelShader();

		void Destroy();

		void Create( me::PixelShaderParameters parameters );

		const me::shader::ShaderConstants * GetConstants() const override;

		void LockConstants( size_t buffer, unify::DataLock & lock ) override;	   

		void UnlockConstants( size_t buffer, unify::DataLock & lock ) override;

		void Use() override;

		void SetTrans( bool bTrans ) override;

		bool IsTrans() override;

		std::string GetError();

	protected:
		me::PixelShaderParameters m_parameters;
		bool m_assembly;
		bool m_created;
		bool m_isTrans;	// Does this pixel shader turn the render into transparent (in part or entire)

		const Renderer * m_renderer;
		CComPtr< ID3D11PixelShader > m_pixelShader;
		CComPtr< ID3D10Blob > m_pixelShaderBuffer;

		me::shader::ShaderConstants::ptr m_constants;
		std::vector< ID3D11Buffer * > m_constantBuffers;
		size_t m_bufferAccessed;
		size_t m_locked;
	};
}