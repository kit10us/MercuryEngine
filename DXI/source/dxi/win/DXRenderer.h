// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/RenderInfo.h>
#include <dxi/core/Display.h>
#include <dxi/win/DirectX.h>
#include <memory>

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

#if defined(DIRECTX9)
			IDirect3DDevice9 * GetDxDevice() const;
#elif defined(DIRECTX11)
			ID3D11Device * GetDxDevice() const;
			ID3D11DeviceContext * GetDxContext() const;
			IDXGISwapChain * GetSwapChain() const;
#endif

			void SetCullMode( CullMode::TYPE mode ) override;

			void BeforeRender() override;
			void AfterRender() override;

			Viewport GetViewport() const override;

			bool IsFullscreen() const override;

			HWND GetHandle() const override;

		private:
			class Pimpl;
			std::shared_ptr< Pimpl > m_pimpl;
		};
	}
}