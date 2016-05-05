// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IRenderer.h>
#include <dxi/RenderInfo.h>

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

		private:
			win::WindowsOS * m_OS;
			RenderInfo m_renderInfo2D;
			RenderInfo m_renderInfo3D;
		};
	}
}