// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/RenderInfo.h>
#include <dxi/core/Display.h>
#include <dxi/win/DirectX.h>
#include <atlbase.h>

namespace dxi
{
	namespace win
	{
		class WindowsOS;

		class DXRenderer : public core::IRenderer
		{
		public:
			DXRenderer( WindowsOS * os, core::Display display );
			virtual ~DXRenderer();

			const core::Display & GetDisplay() const;

			void SetPP( D3DPRESENT_PARAMETERS pp );
			const D3DPRESENT_PARAMETERS & GetPP() const;

			void SetDxDevice( IDirect3DDevice9 * dxDevice );
			IDirect3DDevice9 * GetDxDevice() const;

			void SetSwapChain( IDirect3DSwapChain9 * swapChain );
			IDirect3DSwapChain9 * GetSwapChain() const;

			void SetCullMode( CullMode::TYPE mode ) override;

			void BeforeRender() override;
			void AfterRender() override;

			Viewport GetViewport() const override;

			bool IsFullscreen() const override;

			HWND GetHandle() const override;

		private:
			win::WindowsOS * m_OS;
			core::Display m_display;

#if defined( DIRECTX9 )
			D3DPRESENT_PARAMETERS m_pp;
			CComPtr< IDirect3DDevice9 > m_dxDevice;
			CComPtr< IDirect3DSwapChain9 > m_swapChain;
#elif defined( DIRECTX11 )
			CComPtr< ID3D11DeviceContext > m_dxContext;
			CComPtr< ID3D11Device > m_dxDevice;
			DXGI_SWAP_CHAIN_DESC m_swapChainDesc;
			CComPtr< IDXGISwapChain > m_swapChain;
			CComPtr< ID3D11RenderTargetView > m_renderTargetView;
			CComPtr< ID3D11Texture2D > m_depthStencilBuffer;
			CComPtr< ID3D11DepthStencilView > m_depthStencilView;
			CComPtr< ID3D11RasterizerState > m_rasterizerState;
#endif
		};
	}
}