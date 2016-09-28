// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IRenderer.h>
#include <dxi/RenderInfo.h>
#include <atlbase.h>

namespace dxi
{
	namespace win
	{
		class WindowsOS;

		class DXRenderer : public core::IRenderer
		{
		public:
			DXRenderer( WindowsOS * os );
			virtual ~DXRenderer();

			virtual size_t GetNumberOfViewports() const;
			virtual void GetViewport( Viewport & viewport );
			virtual void SetViewport( const Viewport & viewport );
			virtual void GetViewports( size_t & numberOfViewports, Viewport * viewports );
			virtual void SetViewports( size_t & numberOfViewports, const Viewport * viewports );

			void CreateDirectX();
			void DestroyDirectX();

			void BeforeRender() override;
			void AfterRender() override;

			void SetCullMode( CullMode::TYPE mode ) override;

#if defined( DIRECTX9 )
			IDirect3DDevice9 * GetDxDevice();
#elif defined( DIRECTX11)
#endif

		private:
			win::WindowsOS * m_OS;
			RenderInfo m_renderInfo2D;
			RenderInfo m_renderInfo3D;

#if defined( DIRECTX9 )
			CComPtr< IDirect3DDevice9 > m_dxDevice;
			D3DPRESENT_PARAMETERS m_pp;
#elif defined( DIRECTX11 )
#endif
		};
	}
}