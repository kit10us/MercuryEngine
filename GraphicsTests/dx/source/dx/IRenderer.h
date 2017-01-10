#pragma once

#include <dx/Common.h>

namespace dx
{
	class IRenderer
	{
	public:
		virtual ~IRenderer(){};
		virtual ID3D11DeviceContext * GetDxContext() = 0;
		virtual ID3D11Device * GetDxDevice() = 0;
		virtual DXGI_SWAP_CHAIN_DESC * GetSwapChainDesc() = 0;
		virtual IDXGISwapChain * GetSwapChain() = 0;
		virtual ID3D11RenderTargetView * GetRenderTargetView() = 0;
		virtual ID3D11DepthStencilView * GetDepthStencilView() = 0;
		virtual D3D11_VIEWPORT * GetViewport() = 0;
		virtual ID3D11RasterizerState *  GetRasterizerState() = 0;

		static void SetInstance( IRenderer * renderer );
		static IRenderer * GetInstance();
	protected:
		static IRenderer * s_renderer;
	};
}