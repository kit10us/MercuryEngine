// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/win/DirectX.h>

namespace dxi
{
	class Viewport
	{
	public:
		Viewport();
		Viewport( float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth );

		void SetTopLeftX( float topLeftX );
		void SetTopLeftY( float topLeftY );
		void SetWidth( float width );
		void SetHeight( float height );
		void SetMinDepth( float minDepth );
		void SetMaxDepth( float maxDepth );

		float GetTopLeftX() const;
		float GetTopLeftY() const;
		float GetWidth() const;
		float GetHeight() const;
		float GetMinDepth() const;
		float GetMaxDepth() const;
	private:
#if defined( DIRECTX9 )
		D3DVIEWPORT9 m_dxViewport;
#elif defined( DIRECTX11 )
		D3D11_VIEWPORT m_dxViewport;
#endif
	};
}