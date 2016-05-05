// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

inline
Plane::Plane()
{
}

inline
Plane::Plane( float a, float b, float c, float d )
: a( a )
, b( b )
, c( c )
, d( d )
{
}

/*
inline
float Plane::DotCoord( const V3 * pV ) const
{
	return D3DXPlaneDotCoord( (D3DXPLANE*)this, (D3DXVECTOR3*)pV );
}

inline
float Plane::DotNormal( const V3 * pV ) const
{
	return D3DXPlaneDotNormal( (D3DXPLANE*)this, (D3DXVECTOR3*)pV );
}
*/

