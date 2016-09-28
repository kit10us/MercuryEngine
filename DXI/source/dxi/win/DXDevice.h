// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/win/DirectX.h>

namespace dxi
{
	namespace win
	{
		// A location to share all DirectX common interfaces.
		struct DX
		{
#if defined( DIRECTX9 )
			static void SetDxDevice( IDirect3DDevice9 * dxDevice );
			static IDirect3DDevice9 * GetDxDevice();
#elif defined( DIRECTX11 )
			static void SetDxDevice( ID3D11Device * dxDevice );
			static ID3D11Device * GetDxDevice();

			static void SetDxContext( ID3D11DeviceContext * dxContext );
			static ID3D11DeviceContext * GetDxContext();
#endif
		};
	}
}