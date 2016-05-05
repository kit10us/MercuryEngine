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
			static void SetDxDevice( IDirect3DDevice9 * dxDevice );
			static IDirect3DDevice9 * GetDxDevice();
		};
	}
}