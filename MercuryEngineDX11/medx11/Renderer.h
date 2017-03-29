// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <medx11/DirectX.h>
#include <me/IRenderer.h>
#include <me/render/Display.h>
#include <atlbase.h>
#include <memory>

namespace medx11
{
	class WindowsOS;

	class Renderer : public me::IRenderer
	{
	public:
		Renderer( WindowsOS * os, me::render::Display display, size_t index );
		virtual ~Renderer();				

		ID3D11Device * GetDxDevice() const;
		ID3D11DeviceContext * GetDxContext() const;
		IDXGISwapChain * GetSwapChain() const;


	public: // IRenderer...
		me::IGame* GetGame() override;

		const me::render::Display & GetDisplay() const;

		me::render::Viewport GetViewport() const override;

		void BeforeRender() override;
		void BeforeRenderSolids() override;
		void BeforeRenderTrans() override;
		void AfterRender() override;


		bool IsFullscreen() const override;

		size_t GetIndex() const override;

		void* GetHandle() const override;

		void Render( const me::RenderMethod & method, const me::RenderInfo & renderInfo, me::render::MatrixFeed & matrixFeed ) override;

		me::IVertexBuffer::ptr ProduceVB( me::VertexBufferParameters parameters ) override;
		me::IIndexBuffer::ptr ProduceIB( me::IndexBufferParameters parameters ) override;
		me::IVertexShader::ptr ProduceVS( me::VertexShaderParameters parameters ) override;
		me::IPixelShader::ptr ProducePS( me::PixelShaderParameters parameters ) override;
		me::ITexture::ptr ProduceT( me::TextureParameters parameters ) override;
 		me::IVertexConstruct::ptr ProduceVC( const me::VertexDeclaration & vd, const me::IVertexShader & vs ) override;

		void UseTextures( std::vector< me::ITexture::ptr > textures ) override;

	private:
		WindowsOS * m_os;
		me::render::Display m_display;
		size_t m_index;

		CComPtr< ID3D11Device > m_dxDevice;
		CComPtr< ID3D11DeviceContext > m_dxContext;
		DXGI_SWAP_CHAIN_DESC m_swapChainDesc;
		CComPtr< IDXGISwapChain > m_swapChain;
		CComPtr< ID3D11RenderTargetView > m_renderTargetView;
		CComPtr< ID3D11Texture2D > m_depthStencilBuffer;
		CComPtr< ID3D11DepthStencilView > m_depthStencilView;
		CComPtr< ID3D11RasterizerState > m_rasterizerState;	

		CComPtr< ID3D11DepthStencilState> m_depthStencilState_Solids;
		CComPtr< ID3D11DepthStencilState> m_depthStencilState_Trans;

		size_t m_totalInstances;
		CComPtr< ID3D11Buffer > m_instanceBufferM[ 2 ];
	};
}