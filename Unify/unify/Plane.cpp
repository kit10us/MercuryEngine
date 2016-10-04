// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <unify/Plane.h>

using namespace unify;

Plane Plane::PlaneFromPoints( const unify::V3< float > & v1, const unify::V3< float > & v2, const unify::V3< float > & v3 )
{
	V3< float > v21( v1 - v2 );
	V3< float > v31( v1 - v3 );

	V3< float > n = V3< float >::V3Cross( v21, v31 );

	Plane out( n.x, n.y, n.z, n.x  * v1.x + n.y * v1.y + n.z * v1.z );
	return out;
/*
#ifdef DIRECTX9
	D3DXPlaneFromPoints( (D3DXPLANE*)&out, (D3DXVECTOR3*)&v1, (D3DXVECTOR3*)&v2, (D3DXVECTOR3*)&v3 );
#else
	DirectX::FXMVECTOR dxv[3] =
	{
		{ v1.x, v1.y, v1.z, 1.0f },
		{ v2.x, v2.y, v2.z, 1.0f },
		{ v3.x, v3.y, v3.z, 1.0f }
	};
	DirectX::XMPlaneFromPoints( dxv[0], dxv[1], dxv[2] );
#endif
*/
}

Plane::Plane()
{
}

Plane::Plane( float a, float b, float c, float d )
: a( a )
, b( b )
, c( c )
, d( d )
{
}

float Plane::DotCoord( const V3< float > & v2 ) const
{
	unify::V4< float > v1( a, b, c, d );
	unify::V4< float > v2x4( v2, 1.0f );
	return v1.Dot( v2x4 );
}

float Plane::DotNormal( const V3< float > & v2 ) const
{
	unify::V4< float > v1( a, b, c, d );
	unify::V4< float > v2x4( v2, 0.0f );
	return v1.Dot( v2x4 );
}

bool Plane::IntersectLine( unify::V3< float > & out, const unify::V3< float > & v1, const unify::V3< float > & v2 ) const
{
	assert( 0 );
	/*
#ifdef DIRECTX9
	return D3DXPlaneIntersectLine( (D3DXVECTOR3*)&out, (D3DXPLANE*)&plane, (D3DXVECTOR3*)&v1, (D3DXVECTOR3*)&v2 ) != 0;
#else
	DirectX::XMVECTOR dxPlane = { plane.a, plane.b, plane.c, plane.d };
	DirectX::XMVECTOR dxLinePoint1 = { v1.x, v1.y, v1.z, 1.0f };
	DirectX::XMVECTOR dxLinePoint2 = { v2.x, v2.y, v2.z, 1.0f };
	DirectX::XMVECTOR dxOut = DirectX::XMPlaneIntersectLine( dxPlane, dxLinePoint1, dxLinePoint2 );
	out.x = dxOut.m128_f32[0];
	out.y = dxOut.m128_f32[1];
	out.z = dxOut.m128_f32[2];
	return dxOut.m128_f32[0] != DirectX::g_XMQNaN.v.m128_f32[0];
#endif
*/
	return false;
}
