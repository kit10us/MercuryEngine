// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IRenderer.h>
#include <me/Display.h>
#include <medx9/DirectX.h>
#include <medx9/WindowsOS.h>
#include <atlbase.h>
#include <memory>

namespace medx9
{
	class WindowsOS;

	class Renderer : public me::IRenderer
	{
	public:
		Renderer( WindowsOS * os, me::Display display, size_t index );
		virtual ~Renderer();					

		me::IGame * GetGame() override;

		const me::Display & GetDisplay() const;

		IDirect3DDevice9 * GetDxDevice() const;

		void SetCullMode( me::CullMode::TYPE mode ) override;

		void BeforeRender() override;
		void AfterRender() override;

		me::Viewport GetViewport() const override;

		bool IsFullscreen() const override;

		size_t GetIndex() const override;

		void* GetHandle() const override;
													  
		void Render( const me::RenderMethod & method, const me::RenderInfo & renderInfo, me::MatrixFeed & matrixFeed ) override;

		me::ISketcher * GetSketcher() override;

		me::IVertexBuffer::ptr ProduceVB( me::VertexBufferParameters parameters ) override;
		me::IIndexBuffer::ptr ProduceIB( me::IndexBufferParameters parameters ) override;
		me::IVertexShader::ptr ProduceVS( me::VertexShaderParameters parameters ) override;
		me::IPixelShader::ptr ProducePS( me::PixelShaderParameters parameters ) override;
		me::IVertexConstruct::ptr ProduceVC( const me::VertexDeclaration & vd, const me::IVertexShader & vs ) override;
		me::ITexture::ptr ProduceT( me::TextureParameters parameters ) override;

		void UseTextures( std::vector< me::ITexture::ptr > textures ) override;

	private:
		WindowsOS * m_os;
		me::Display m_display;
		D3DPRESENT_PARAMETERS m_pp;
		CComPtr< IDirect3DDevice9 > m_dxDevice;
		CComPtr< IDirect3DSwapChain9 > m_swapChain;
		size_t m_index;
	};
}