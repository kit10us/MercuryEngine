// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IRenderer.h>
#include <me/Display.h>
#include <medx9/DirectX.h>
#include <memory>

namespace medx9
{
	class WindowsOS;

	class Renderer : public me::IRenderer
	{
	public:
		Renderer( WindowsOS * os, me::Display display, size_t index );
		virtual ~Renderer();																															   
		const me::Display & GetDisplay() const;

		IDirect3DDevice9 * GetDxDevice() const;

		void SetCullMode( me::CullMode::TYPE mode ) override;

		void BeforeRender() override;
		void AfterRender() override;

		me::Viewport GetViewport() const override;

		bool IsFullscreen() const override;

		size_t GetIndex() const override;

		void* GetHandle() const override;
													  
		void Render( const me::RenderMethod & method, const me::RenderInfo & renderInfo, const me::RenderInstance & instance ) const override;

		void RenderInstanced( const me::RenderMethod & method, const me::RenderInfo & renderInfo, const std::list< me::RenderInstance > & instances ) const override;

		me::IVertexBuffer::ptr ProduceVB( me::VertexBufferParameters parameters ) const override;
		me::IIndexBuffer::ptr ProduceIB( me::IndexBufferParameters parameters ) const override;
		me::IVertexShader::ptr ProduceVS( me::VertexShaderParameters parameters ) const override;
		me::IPixelShader::ptr ProducePS( me::PixelShaderParameters parameters ) const override;
		me::IVertexConstruct::ptr ProduceVC( const me::VertexDeclaration & vd, const me::IVertexShader & vs ) const override;
		me::ITexture::ptr ProduceT( me::TextureParameters parameters ) const override;

	private:
		class Pimpl;
		std::shared_ptr< Pimpl > m_pimpl;
	};
}