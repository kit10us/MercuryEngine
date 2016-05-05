// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>

namespace unify
{
	class Plane
	{
	public:
		float a, b, c, d;

		Plane();
		Plane( float a, float b, float c, float d );

		//float DotCoord( const V3 * pV ) const;
		//float DotNormal( const V3 * pV ) const;
	};

	inline void PlaneFromPoints( unify::Plane & out, const unify::V3< float > & v1, const unify::V3< float > & v2, const unify::V3< float > & v3 )
	{
#ifdef DIRECTX9
		D3DXPlaneFromPoints( (D3DXPLANE*)&out, (D3DXVECTOR3*)&v1, (D3DXVECTOR3*)&v2, (D3DXVECTOR3*)&v3 );
#else
		DirectX::FXMVECTOR dxv[3] =
		{
			{ v1.x, v1.y, v1.z, 1.0f },
			{ v2.x, v2.y, v2.z, 1.0f },
			{ v3.x, v3.y, v3.z, 1.0f }
		};
		DirectX::XMPlaneFromPoints( dxv[ 0 ], dxv[ 1 ], dxv[ 2 ] );
#endif
	}

	inline bool PlaneIntersectLine( unify::V3< float > & out, const unify::Plane & plane, const unify::V3< float > & v1, const unify::V3< float > & v2 )
	{
#ifdef DIRECTX9
		return D3DXPlaneIntersectLine( (D3DXVECTOR3*)&out, (D3DXPLANE*)&plane, (D3DXVECTOR3*)&v1, (D3DXVECTOR3*)&v2 ) != 0;
#else
		DirectX::XMVECTOR dxPlane = { plane.a, plane.b, plane.c, plane.d };
		DirectX::XMVECTOR dxLinePoint1 = { v1.x, v1.y, v1.z, 1.0f };
		DirectX::XMVECTOR dxLinePoint2 = { v2.x, v2.y, v2.z, 1.0f };
		DirectX::XMVECTOR dxOut = DirectX::XMPlaneIntersectLine( dxPlane, dxLinePoint1, dxLinePoint2 );
		out.x = dxOut.m128_f32[ 0 ];
		out.y = dxOut.m128_f32[ 1 ];
		out.z = dxOut.m128_f32[ 2 ];
		return dxOut.m128_f32[ 0 ] != DirectX::g_XMQNaN.v.m128_f32[ 0 ];
#endif
	}
	#include <unify/Plane.inl>
}
