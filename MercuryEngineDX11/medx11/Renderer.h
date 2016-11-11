// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IRenderer.h>
#include <me/Display.h>
#include <medx11/DirectX.h>
#include <atlbase.h>
#include <memory>

namespace medx11
{
	class WindowsOS;

	class Renderer : public me::IRenderer
	{
	public:
		Renderer( WindowsOS * os, me::Display display, size_t index );
		virtual ~Renderer();																															   
		const me::Display & GetDisplay() const;

		ID3D11Device * GetDxDevice() const;
		ID3D11DeviceContext * GetDxContext() const;
		IDXGISwapChain * GetSwapChain() const;

		void SetCullMode( me::CullMode::TYPE mode ) override;

		void BeforeRender() override;
		void AfterRender() override;

		me::Viewport GetViewport() const override;

		bool IsFullscreen() const override;

		size_t GetIndex() const override;

		void* GetHandle() const override;


		void Render( me::RenderMethod ) const override;

		me::IVertexBuffer::ptr ProduceVB( me::VertexBufferParameters parameters ) override;
		me::IIndexBuffer::ptr ProduceIB( me::IndexBufferParameters parameters ) override;
		me::IVertexShader::ptr ProduceVS( me::VertexShaderParameters parameters ) override;
		me::IPixelShader::ptr ProducePS( me::PixelShaderParameters parameters ) override;
		me::IVertexDeclaration::ptr ProduceVD( me::VertexDeclarationParameters parameters ) override;
		me::ITexture::ptr ProduceT( me::TextureParameters parameters ) override;

	private:
	WindowsOS * m_OS;
	me::Display m_display;
	size_t m_index;

	CComPtr< ID3D11Device > m_dxDevice;
	CComPtr< ID3D11DeviceContext > m_dxContext;
	DXGI_SWAP_CHAIN_DESC m_swapChainDesc;
	CComPtr< IDXGISwapChain > m_swapChain;
	CComPtr< ID3D11RenderTargetView > m_renderTargetView;
	CComPtr< ID3D11Texture2D > m_depthStencilBuffer;
	CComPtr< ID3D11DepthStencilView > m_depthStencilView;
	CComPtr< ID3D11RasterizerState > m_rasterizerState;	};
}