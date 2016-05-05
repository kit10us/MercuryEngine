// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>

#if DIRECT3D_VERSION == 0x0900
#define WIN32_LEAN_AND_MEAN
#include <d3dx9math.h>
#endif

namespace unify
{
	namespace dx
	{
#if DIRECT3D_VERSION == 0x0900
		D3DXMATRIX * Convert( Matrix & matrix )
		{
			return matrix.Raw();
		}

		const D3DXMATRIX * Convert( const Matrix & matrix ) const
		{
			return matrix.Raw();
		}
#endif
	}
}