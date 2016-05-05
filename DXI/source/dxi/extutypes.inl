////////////////////////////
// unify::V2
////////////////////////////

inline unify::V2< unify::TFloat >* qV2Add( unify::V2< unify::TFloat >* pOut, const unify::V2< unify::TFloat >* pV1, const unify::V2< unify::TFloat >* pV2 ) 
{
	return (unify::V2< unify::TFloat >*)D3DXVec2Add( (D3DXVECTOR2*) pOut, (D3DXVECTOR2*) pV1, (D3DXVECTOR2*) pV2 ); 
}

inline unify::V2< unify::TFloat >* qV2BaryCentric( unify::V2< unify::TFloat >* pOut, const unify::V2< unify::TFloat >* pV1, const unify::V2< unify::TFloat >* pV2, const unify::V2< unify::TFloat >* pV3, unify::TFloat f, unify::TFloat g )
{
	return (unify::V2< unify::TFloat >*)D3DXVec2BaryCentric( (D3DXVECTOR2*) pOut, (D3DXVECTOR2*) pV1, (D3DXVECTOR2*) pV2, (D3DXVECTOR2*) pV3, f, g ); 
}

inline unify::V2< unify::TFloat >* qV2CatmullRom( unify::V2< unify::TFloat >* pOut, const unify::V2< unify::TFloat >* pV1, const unify::V2< unify::TFloat >* pV2, const unify::V2< unify::TFloat >* pV3, const unify::V2< unify::TFloat >* pV4, unify::TFloat s )
{
	return (unify::V2< unify::TFloat >*)D3DXVec2CatmullRom( (D3DXVECTOR2*) pOut, (D3DXVECTOR2*) pV1, (D3DXVECTOR2*) pV2, (D3DXVECTOR2*) pV3, (D3DXVECTOR2*) pV4, s );
}

inline unify::TFloat qV2CCW( const unify::V2< unify::TFloat >* pV1, const unify::V2< unify::TFloat >* pV2 ) 
{
	return D3DXVec2CCW( (D3DXVECTOR2*)pV1,	(D3DXVECTOR2*)pV2 );
}

inline unify::TFloat qV2Dot( const unify::V2< unify::TFloat >* pV1, const unify::V2< unify::TFloat >* pV2 )
{
	return D3DXVec2Dot( (D3DXVECTOR2*)pV1, (D3DXVECTOR2*)pV2 );
}

inline unify::V2< unify::TFloat >* qV2Hermite( unify::V2< unify::TFloat >* pOut, const unify::V2< unify::TFloat >* pV1, const unify::V2< unify::TFloat >* pT1, const unify::V2< unify::TFloat >* pV2, const unify::V2< unify::TFloat >* pT2, unify::TFloat s )
{
	return (unify::V2< unify::TFloat >*)D3DXVec2Hermite( (D3DXVECTOR2*)pOut, (D3DXVECTOR2*)pV1, (D3DXVECTOR2*)pT1, (D3DXVECTOR2*)pV2, (D3DXVECTOR2*)pT2, s );
}

inline unify::TFloat qV2Length( const unify::V2< unify::TFloat >* pV )
{
	return D3DXVec2Length( (D3DXVECTOR2*) pV ); 
}

inline unify::TFloat qV2LengthSq( const unify::V2< unify::TFloat >* pV )
{
	return D3DXVec2LengthSq( (D3DXVECTOR2*)pV );
}

inline unify::V2< unify::TFloat >* qV2Lerp( unify::V2< unify::TFloat >* pOut, const unify::V2< unify::TFloat >* pV1, const unify::V2< unify::TFloat >* pV2, unify::TFloat s )
{
	return (unify::V2< unify::TFloat >*)D3DXVec2Lerp( (D3DXVECTOR2*)pOut, (D3DXVECTOR2*)pV1, (D3DXVECTOR2*)pV2, s ); 
}

inline unify::V2< unify::TFloat >* qV2Maximize( unify::V2< unify::TFloat >* pOut, const unify::V2< unify::TFloat >* pV1, const unify::V2< unify::TFloat >* pV2 )
{
	return (unify::V2< unify::TFloat >*)D3DXVec2Maximize( (D3DXVECTOR2*)pOut, (D3DXVECTOR2*)pV1, (D3DXVECTOR2*)pV2	); 
}

inline unify::V2< unify::TFloat >* qV2Minimize( unify::V2< unify::TFloat >* pOut, const unify::V2< unify::TFloat >* pV1, const unify::V2< unify::TFloat >* pV2 ) 
{
	return (unify::V2< unify::TFloat >*)D3DXVec2Minimize( (D3DXVECTOR2*)pOut, (D3DXVECTOR2*)pV1, (D3DXVECTOR2*)pV2 ); 
}

inline unify::V2< unify::TFloat >* qV2Normalize( unify::V2< unify::TFloat >* pOut, const unify::V2< unify::TFloat >* pV ) 
{
	return (unify::V2< unify::TFloat >*)D3DXVec2Normalize( (D3DXVECTOR2*)pOut, (D3DXVECTOR2*)pV );
}

inline unify::V2< unify::TFloat >* qV2Scale( unify::V2< unify::TFloat >* pOut, const unify::V2< unify::TFloat >* pV, unify::TFloat s )
{
	return (unify::V2< unify::TFloat >*)D3DXVec2Scale( (D3DXVECTOR2*)pOut, (D3DXVECTOR2*)pV, s	);
}

inline unify::V2< unify::TFloat >* qV2Subtract( unify::V2< unify::TFloat >* pOut, const unify::V2< unify::TFloat >* pV1, const unify::V2< unify::TFloat >* pV2 )
{
	return (unify::V2< unify::TFloat >*)D3DXVec2Subtract( (D3DXVECTOR2*)pOut, (D3DXVECTOR2*)pV1, (D3DXVECTOR2*)pV2 );
}

inline unify::V4< unify::TFloat >* qV2Transform( unify::V4< unify::TFloat >* pOut, const unify::V2< unify::TFloat >* pV, const TMatrix* pM )
{
	return (unify::V4< unify::TFloat >*)D3DXVec2Transform( (D3DXVECTOR4*)pOut, (D3DXVECTOR2*) pV, (D3DXMATRIX*) pM );
}

inline unify::V2< unify::TFloat >* qV2TransformCoord( unify::V2< unify::TFloat >* pOut, const unify::V2< unify::TFloat >* pV, const TMatrix* pM )
{
	return (unify::V2< unify::TFloat >*)D3DXVec2TransformCoord( (D3DXVECTOR2*)pOut, (D3DXVECTOR2*)pV, (D3DXMATRIX*)pM );
}

inline unify::V2< unify::TFloat >* qV2TransformNormal( unify::V2< unify::TFloat >* pOut, const unify::V2< unify::TFloat >* pV, const TMatrix* pM )
{
	return (unify::V2< unify::TFloat >*)D3DXVec2TransformNormal( (D3DXVECTOR2*)pOut, (D3DXVECTOR2*)pV, (D3DXMATRIX*)pM );
}

inline unify::V4< unify::TFloat >* qV2TransformArray( unify::V4< unify::TFloat > *pOut, unify::TUInt32 OutStride, const unify::V2< unify::TFloat >* pV, unify::TUInt32 VStride, const TMatrix* pM, unify::TUInt32 n )
{
	return (unify::V4< unify::TFloat >*)D3DXVec2TransformArray( (D3DXVECTOR4*)pOut, OutStride, (D3DXVECTOR2*)pV, VStride, (D3DXMATRIX*)pM, n );
}

inline unify::V2< unify::TFloat >* qV2TransformCoordArray( unify::V2< unify::TFloat >* pOut, unify::TUInt32 OutStride, const unify::V2< unify::TFloat >* pV, unify::TUInt32 VStride, const TMatrix *pM, unify::TUInt32 n )
{
	return (unify::V2< unify::TFloat >*)D3DXVec2TransformCoordArray( (D3DXVECTOR2*)pOut, OutStride, (D3DXVECTOR2*)pV, VStride, (D3DXMATRIX*)pM, n );
}

inline unify::V2< unify::TFloat >* qV2TransformNormalArray( unify::V2< unify::TFloat >* pOut, unify::TUInt32 OutStride, const unify::V2< unify::TFloat >* pV, unify::TUInt32 VStride, const TMatrix *pM, unify::TUInt32 n )
{
	return (unify::V2< unify::TFloat >*)D3DXVec2TransformNormalArray( (D3DXVECTOR2*)pOut, OutStride, (D3DXVECTOR2*)pV, VStride, (D3DXMATRIX*)pM, n );
}


//////////////////////////////////////////////
// unify::V4
//////////////////////////////////////////////

// Non-member Functions...
inline unify::V3< unify::TFloat > * qV3Add( unify::V3< unify::TFloat > * pOut, const unify::V3< unify::TFloat > * pV1, const unify::V3< unify::TFloat > * pV2 )
{
	return (unify::V3< unify::TFloat > *)D3DXVec3Add( (D3DXVECTOR3*)pOut, (D3DXVECTOR3*)pV1, (D3DXVECTOR3*)pV2 );
}

inline unify::V3< unify::TFloat > * qV3BaryCentric( unify::V3< unify::TFloat > * pOut, const unify::V3< unify::TFloat > * pV1, const unify::V3< unify::TFloat > * pV2, const unify::V3< unify::TFloat > * pV3, unify::TFloat f, unify::TFloat g )
{
	return (unify::V3< unify::TFloat > *)D3DXVec3BaryCentric( (D3DXVECTOR3*)pOut, (D3DXVECTOR3*)pV1, (D3DXVECTOR3*)pV2, (D3DXVECTOR3*)pV3, f, g ); 
}

inline unify::V3< unify::TFloat > * qV3CatmullRom( unify::V3< unify::TFloat > * pOut, const unify::V3< unify::TFloat > * pV1, const unify::V3< unify::TFloat > * pV2, const unify::V3< unify::TFloat > * pV3, const unify::V3< unify::TFloat > * pV4, unify::TFloat s )
{
	return (unify::V3< unify::TFloat > *)D3DXVec3CatmullRom( (D3DXVECTOR3*)pOut, (D3DXVECTOR3*)pV1, (D3DXVECTOR3*)pV2, (D3DXVECTOR3*)pV3, (D3DXVECTOR3*)pV4, s ); 
}

inline unify::V3< unify::TFloat > * qV3Cross( unify::V3< unify::TFloat > * pOut, const unify::V3< unify::TFloat > * pV1, const unify::V3< unify::TFloat > * pV2 )
{
	return (unify::V3< unify::TFloat > *)D3DXVec3Cross( (D3DXVECTOR3*)pOut, (D3DXVECTOR3*)pV1, (D3DXVECTOR3*)pV2 ); 
}

inline unify::TFloat qV3Dot( const unify::V3< unify::TFloat > * pV1, const unify::V3< unify::TFloat > * pV2 )
{
	return D3DXVec3Dot( (D3DXVECTOR3*)pV1, (D3DXVECTOR3*)pV2 ); 
}

inline unify::V3< unify::TFloat > * qV3Hermite( unify::V3< unify::TFloat > * pOut, const unify::V3< unify::TFloat > * pV1, const unify::V3< unify::TFloat > * pT1, const unify::V3< unify::TFloat > * pV2, const unify::V3< unify::TFloat > * pT2, unify::TFloat s )
{
	return (unify::V3< unify::TFloat > *)D3DXVec3Hermite( (D3DXVECTOR3*)pOut, (D3DXVECTOR3*)pV1, (D3DXVECTOR3*)pT1, (D3DXVECTOR3*)pV2, (D3DXVECTOR3*)pT2, s );
}

inline unify::TFloat qV3Length( const unify::V3< unify::TFloat > * pV )
{
	return D3DXVec3Length( (D3DXVECTOR3*)pV );
}

inline unify::TFloat qV3LengthSq( const unify::V3< unify::TFloat > * pV )
{
	return D3DXVec3LengthSq( (D3DXVECTOR3*)pV );
}

inline unify::V3< unify::TFloat > * qV3Lerp( unify::V3< unify::TFloat > * pOut, const unify::V3< unify::TFloat > * pV1, const unify::V3< unify::TFloat > * pV2, unify::TFloat s )
{
	return (unify::V3< unify::TFloat > *)D3DXVec3Lerp( (D3DXVECTOR3*)pOut, (D3DXVECTOR3*)pV1, (D3DXVECTOR3*)pV2, s );
}

inline unify::V3< unify::TFloat > * qV3Maximize( unify::V3< unify::TFloat > * pOut, const unify::V3< unify::TFloat > * pV1, const unify::V3< unify::TFloat > * pV2 )
{
	return (unify::V3< unify::TFloat > *)D3DXVec3Maximize( (D3DXVECTOR3*)pOut, (D3DXVECTOR3*)pV1, (D3DXVECTOR3*)pV2 );
}

inline unify::V3< unify::TFloat > * qV3Minimize( unify::V3< unify::TFloat > * pOut, const unify::V3< unify::TFloat > * pV1, const unify::V3< unify::TFloat > * pV2 )
{
	return (unify::V3< unify::TFloat > *)D3DXVec3Minimize( (D3DXVECTOR3*)pOut, (D3DXVECTOR3*)pV1, (D3DXVECTOR3*)pV2 );
}

inline unify::V3< unify::TFloat > * qV3Normalize( unify::V3< unify::TFloat > * dest, const unify::V3< unify::TFloat > * src )
{
	return (unify::V3< unify::TFloat > *)D3DXVec3Normalize( (D3DXVECTOR3*)dest, (D3DXVECTOR3*)src );
}

inline unify::V3< unify::TFloat > * qV3Scale( unify::V3< unify::TFloat > * pOut, const unify::V3< unify::TFloat > * pV, unify::TFloat s )
{
	return (unify::V3< unify::TFloat > *)D3DXVec3Scale( (D3DXVECTOR3*)pOut, (D3DXVECTOR3*)pV, s );
}

inline unify::V3< unify::TFloat > * qV3Subtract( unify::V3< unify::TFloat > * pOut, const unify::V3< unify::TFloat > * pV1, const unify::V3< unify::TFloat > * pV2 )
{
	return (unify::V3< unify::TFloat > *)D3DXVec3Subtract( (D3DXVECTOR3*)pOut, (D3DXVECTOR3*)pV1, (D3DXVECTOR3*)pV2 );
}

inline unify::V4< unify::TFloat > * qV3Transform( unify::V4< unify::TFloat >* pOut, const unify::V3< unify::TFloat > * pV, const TMatrix* pM )
{
	return (unify::V4< unify::TFloat > *)D3DXVec3Transform( (D3DXVECTOR4*)pOut, (D3DXVECTOR3*)pV, (D3DXMATRIX*)pM );
}

inline unify::V3< unify::TFloat > * qV3TransformCoord( unify::V3< unify::TFloat > * dest, const unify::V3< unify::TFloat > * src, const TMatrix* mat )
{
	return (unify::V3< unify::TFloat > *)D3DXVec3TransformCoord( (D3DXVECTOR3*)dest, (D3DXVECTOR3*)src, &mat->mat );
}

inline unify::V3< unify::TFloat > * qV3TransformNormal( unify::V3< unify::TFloat > * dest, const unify::V3< unify::TFloat > * src, const TMatrix* mat )
{
	return (unify::V3< unify::TFloat > *)D3DXVec3TransformNormal( (D3DXVECTOR3*)dest, (D3DXVECTOR3*)src, &mat->mat );
}

// Compute the distance between two vertices
inline unify::TFloat qV3Distance( const unify::V3< unify::TFloat > * pV1, const unify::V3< unify::TFloat > * pV2 )
{

	return( qV3Length( &(*pV1 - *pV2) ) );
}

inline unify::V4< unify::TFloat > * qV3TransformArray( unify::V4< unify::TFloat > *pOut, unify::TUInt32 OutStride, const unify::V3< unify::TFloat > *pV, unify::TUInt32 VStride, const TMatrix *pM, unify::TUInt32 n )
{
	return (unify::V4< unify::TFloat > *)D3DXVec3TransformArray( (D3DXVECTOR4*)pOut, OutStride, (D3DXVECTOR3*)pV, VStride, (D3DXMATRIX*)pM, n );
}

inline unify::V3< unify::TFloat > * qV3TransformCoordArray( unify::V3< unify::TFloat > * pOut, unify::TUInt32 OutStride, const unify::V3< unify::TFloat > * pV, unify::TUInt32 VStride, const TMatrix* pM, unify::TUInt32 n )
{
	return (unify::V3< unify::TFloat > *)D3DXVec3TransformCoordArray( (D3DXVECTOR3*)pOut, OutStride, (D3DXVECTOR3*)pV, VStride, (D3DXMATRIX*)pM, n );
}

inline unify::V3< unify::TFloat > * qV3TransformNormalArray( unify::V3< unify::TFloat > *pOut, unify::TUInt32 OutStride, const unify::V3< unify::TFloat > *pV, unify::TUInt32 VStride, const TMatrix *pM, unify::TUInt32 n )
{
		return (unify::V3< unify::TFloat > *)D3DXVec3TransformNormalArray( (D3DXVECTOR3*)pOut, OutStride, (D3DXVECTOR3*)pV, VStride, (D3DXMATRIX*)pM, n );
}

// Project vector from object space into screen space
inline unify::V3< unify::TFloat > * qV3Project( unify::V3< unify::TFloat > *pOut, const unify::V3< unify::TFloat > *pV, const unify::Viewport *pViewport,
	  const TMatrix *pProjection, const TMatrix *pView, const TMatrix *pWorld )
{
	return (unify::V3< unify::TFloat > *)D3DXVec3Project( (D3DXVECTOR3*)pOut, (D3DXVECTOR3*)pV, (D3DVIEWPORT9*)pViewport, (D3DXMATRIX*)pProjection, (D3DXMATRIX*)pView, (D3DXMATRIX*)pWorld );
}

// Project vector from screen space into object space
inline unify::V3< unify::TFloat > * qV3Unproject( unify::V3< unify::TFloat > *pOut, const unify::V3< unify::TFloat > *pV, const unify::Viewport *pViewport,
	  const TMatrix *pProjection, const TMatrix *pView, const TMatrix *pWorld )
{
	return (unify::V3< unify::TFloat > *)D3DXVec3Unproject( (D3DXVECTOR3*)pOut, (D3DXVECTOR3*)pV, (D3DVIEWPORT9*)pViewport, (D3DXMATRIX*)pProjection, (D3DXMATRIX*)pView, (D3DXMATRIX*)pWorld );
}
      
// Project vector Array from object space into screen space
inline unify::V3< unify::TFloat > * qV3ProjectArray( unify::V3< unify::TFloat > *pOut, unify::TUInt32 OutStride, const unify::V3< unify::TFloat > *pV, unify::TUInt32 VStride, const unify::Viewport *pViewport,
	  const TMatrix *pProjection, const TMatrix *pView, const TMatrix *pWorld, unify::TUInt32 n )
{
	return (unify::V3< unify::TFloat > *)D3DXVec3ProjectArray( (D3DXVECTOR3*)pOut, OutStride, (D3DXVECTOR3*)pV, VStride,(D3DVIEWPORT9*)pViewport,
	  (D3DXMATRIX*)pProjection, (D3DXMATRIX*)pView, (D3DXMATRIX*)pWorld, n );
}

// Project vector Array from screen space into object space
inline unify::V3< unify::TFloat > * qV3UnprojectArray( unify::V3< unify::TFloat > *pOut, unify::TUInt32 OutStride, const unify::V3< unify::TFloat > *pV, unify::TUInt32 VStride, const unify::Viewport *pViewport,
	  const TMatrix *pProjection, const TMatrix *pView, const TMatrix *pWorld, unify::TUInt32 n )
{
	return (unify::V3< unify::TFloat > *)D3DXVec3UnprojectArray
		( (D3DXVECTOR3*)pOut, OutStride, (D3DXVECTOR3*)pV, VStride, (D3DVIEWPORT9*)pViewport,
		  (D3DXMATRIX*)pProjection, (D3DXMATRIX*)pView, (D3DXMATRIX*)pWorld, n );
}


//////////////////////////////////////////////
// unify::V4
//////////////////////////////////////////////

inline unify::TFloat qV4Length( const unify::V4< unify::TFloat > *pV )
{
	return D3DXVec4Length( (D3DXVECTOR4*)pV );
}

inline unify::TFloat qV4LengthSq( const unify::V4< unify::TFloat >* pV )
{
	return D3DXVec4LengthSq( (D3DXVECTOR4*)pV );
}

inline unify::TFloat qV4Dot( const unify::V4< unify::TFloat >* pV1, const unify::V4< unify::TFloat >* pV2 )
{
	return D3DXVec4Dot( (D3DXVECTOR4*)pV1, (D3DXVECTOR4*)pV2 );
}

inline unify::V4< unify::TFloat > * qV4Add( unify::V4< unify::TFloat >* pOut, const unify::V4< unify::TFloat >* pV1, const unify::V4< unify::TFloat >* pV2 )
{
	return (unify::V4< unify::TFloat > *)D3DXVec4Add( (D3DXVECTOR4*)pOut, (D3DXVECTOR4*)pV1, (D3DXVECTOR4*)pV2);
}

inline unify::V4< unify::TFloat > * qV4Substract( unify::V4< unify::TFloat >* pOut, const unify::V4< unify::TFloat >* pV1, const unify::V4< unify::TFloat >* pV2 )
{
	return (unify::V4< unify::TFloat > *)D3DXVec4Subtract( (D3DXVECTOR4*)pOut, (D3DXVECTOR4*)pV1, (D3DXVECTOR4*)pV2 );
}

inline unify::V4< unify::TFloat > * qV4Minimize( unify::V4< unify::TFloat >* pOut, const unify::V4< unify::TFloat >* pV1, const unify::V4< unify::TFloat >* pV2 )
{
	return (unify::V4< unify::TFloat > *)D3DXVec4Minimize( (D3DXVECTOR4*)pOut, (D3DXVECTOR4*)pV1, (D3DXVECTOR4*)pV2 );
}

inline unify::V4< unify::TFloat > * qV4Maximize( unify::V4< unify::TFloat >* pOut, const unify::V4< unify::TFloat >* pV1, const unify::V4< unify::TFloat >* pV2 )
{
	return (unify::V4< unify::TFloat > *)D3DXVec4Maximize( (D3DXVECTOR4*)pOut, (D3DXVECTOR4*)pV1, (D3DXVECTOR4*)pV2 );
}

inline unify::V4< unify::TFloat > * qV4Scale( unify::V4< unify::TFloat >* pOut, const unify::V4< unify::TFloat >* pV, unify::TFloat s )
{
	return (unify::V4< unify::TFloat > *)D3DXVec4Scale( (D3DXVECTOR4*)pOut, (D3DXVECTOR4*)pV, s );
}

inline unify::V4< unify::TFloat > * qV4Lerp( unify::V4< unify::TFloat >* pOut, const unify::V4< unify::TFloat >* pV1, const unify::V4< unify::TFloat >* pV2, unify::TFloat s )
{
	return (unify::V4< unify::TFloat > *)D3DXVec4Lerp( (D3DXVECTOR4*)pOut, (D3DXVECTOR4*)pV1, (D3DXVECTOR4*)pV2, s );
}

inline unify::V4< unify::TFloat > * qV4Cross( unify::V4< unify::TFloat > *pOut, const unify::V4< unify::TFloat >* pV1, const unify::V4< unify::TFloat >* pV2, const unify::V4< unify::TFloat >* pV3 )
{
	return (unify::V4< unify::TFloat > *)D3DXVec4Cross( (D3DXVECTOR4*)pOut, (D3DXVECTOR4*)pV1, (D3DXVECTOR4*)pV2, (D3DXVECTOR4*)pV3 );
}

inline unify::V4< unify::TFloat > * qV4Normalize( unify::V4< unify::TFloat > *pOut, const unify::V4< unify::TFloat > *pV )
{
	return (unify::V4< unify::TFloat > *)D3DXVec4Normalize( (D3DXVECTOR4*)pOut, (D3DXVECTOR4*)pV );
}

inline unify::V4< unify::TFloat > * qV4Hermite( unify::V4< unify::TFloat >* pOut, const unify::V4< unify::TFloat >* pV1, const unify::V4< unify::TFloat >* pT1, const unify::V4< unify::TFloat >* pV2, const unify::V4< unify::TFloat >* pT2, unify::TFloat s )
{
	return (unify::V4< unify::TFloat > *)D3DXVec4Hermite( (D3DXVECTOR4*)pOut, (D3DXVECTOR4*)pV1, (D3DXVECTOR4*)pT1, (D3DXVECTOR4*)pV2, 
		(D3DXVECTOR4*)pT2, s );
}

inline unify::V4< unify::TFloat > * qV4CatmullRom( unify::V4< unify::TFloat >* pOut, const unify::V4< unify::TFloat >* pV0, const unify::V4< unify::TFloat >* pV1, const unify::V4< unify::TFloat >* pV2, const unify::V4< unify::TFloat >* pV3, unify::TFloat s )
{
	return (unify::V4< unify::TFloat > *)D3DXVec4CatmullRom( (D3DXVECTOR4*)pOut, (D3DXVECTOR4*)pV0, (D3DXVECTOR4*)pV1, (D3DXVECTOR4*)pV2,
		(D3DXVECTOR4*)pV3, s );
}

inline unify::V4< unify::TFloat > * qV4BaryCentric( unify::V4< unify::TFloat >* pOut, const unify::V4< unify::TFloat >* pV1, const unify::V4< unify::TFloat >* pV2, const unify::V4< unify::TFloat >* pV3, unify::TFloat f, unify::TFloat g )
{
	return (unify::V4< unify::TFloat > *)D3DXVec4BaryCentric( (D3DXVECTOR4*)pOut, (D3DXVECTOR4*)pV1, (D3DXVECTOR4*)pV2, 
		(D3DXVECTOR4*)pV3, f, g );
}

inline unify::V4< unify::TFloat > * qV4Transform( unify::V4< unify::TFloat >* pOut, const unify::V4< unify::TFloat >* pV, const TMatrix* pM )
{
	return (unify::V4< unify::TFloat > *)D3DXVec4Transform( (D3DXVECTOR4*)pOut, (D3DXVECTOR4*)pV, (D3DXMATRIX*)pM );
}
    
inline unify::V4< unify::TFloat > * qV4TransformArray( unify::V4< unify::TFloat >* pOut, unify::TUInt32 OutStride, const unify::V4< unify::TFloat >* pV, unify::TUInt32 VStride, const TMatrix* pM, unify::TUInt32 n )
{
	return (unify::V4< unify::TFloat > *)D3DXVec4TransformArray( (D3DXVECTOR4*)pOut, OutStride, (D3DXVECTOR4*)pV, VStride, (D3DXMATRIX*)pM, n );
}


/////
// unify::Quaternion
/////

inline unify::Quaternion* qQuaternionBaryCentric( unify::Quaternion* pOut, const unify::Quaternion* pQ1, const unify::Quaternion* pQ2, const unify::Quaternion* pQ3, unify::TFloat f, unify::TFloat g )
{
	return (unify::Quaternion*)D3DXQuaternionBaryCentric( (D3DXQUATERNION*)pOut, (D3DXQUATERNION*)pQ1, (D3DXQUATERNION*)pQ2, (D3DXQUATERNION*)pQ3, f, g );
}

inline unify::Quaternion* qQuaternionConjugate( unify::Quaternion* pOut, const unify::Quaternion* pQ )
{
	return (unify::Quaternion*)D3DXQuaternionConjugate( (D3DXQUATERNION*)pOut, (D3DXQUATERNION*)pQ );
}

inline unify::TFloat qQuaternionDot( unify::Quaternion* pQ1, const unify::Quaternion* pQ2 )
{
	return D3DXQuaternionDot( (D3DXQUATERNION*)pQ1, (D3DXQUATERNION*)pQ2 );
}

inline unify::Quaternion* qQuaternionExp( unify::Quaternion* pOut, const unify::Quaternion* pQ )
{
	return (unify::Quaternion*)D3DXQuaternionExp( (D3DXQUATERNION*)pOut, (D3DXQUATERNION*)pQ );
}

inline unify::Quaternion* qQuaternionIdentity( unify::Quaternion* pOut )
{
	return (unify::Quaternion*)D3DXQuaternionIdentity( (D3DXQUATERNION*)pOut );
}


inline unify::Quaternion* qQuaternionInverse( unify::Quaternion* pOut, const unify::Quaternion* pQ )
{
	return (unify::Quaternion*)D3DXQuaternionInverse( (D3DXQUATERNION*)pOut, (D3DXQUATERNION*)pQ ); 
}

inline unify::TBool qQuaternionIsIdentity( const unify::Quaternion* pQ )
{
	return D3DXQuaternionIsIdentity( (D3DXQUATERNION*)pQ ) == 0 ? false : true; 
}

inline unify::TFloat qQuaternionLength( const unify::Quaternion* pQ )
{
	return D3DXQuaternionLength( (D3DXQUATERNION*)pQ ); 
}

inline unify::TFloat qQuaternionLengthSq( const unify::Quaternion* pQ )
{
	return D3DXQuaternionLengthSq( (D3DXQUATERNION*)pQ );
}

inline unify::Quaternion* qQuaternionLn( unify::Quaternion* pOut, const unify::Quaternion* pQ )
{
	return (unify::Quaternion*)D3DXQuaternionLn( (D3DXQUATERNION*)pOut, (D3DXQUATERNION*)pQ ); 
}

inline unify::Quaternion* qQuaternionMultiply( unify::Quaternion* pOut, const unify::Quaternion* pQ1, const unify::Quaternion* pQ2 )
{
	return (unify::Quaternion*)D3DXQuaternionMultiply( (D3DXQUATERNION*)pOut, (D3DXQUATERNION*)pQ1, (D3DXQUATERNION*)pQ2 );
}

inline unify::Quaternion* qQuaternionNormalize( unify::Quaternion* pOut, const unify::Quaternion* pQ )
{
	return (unify::Quaternion*)D3DXQuaternionNormalize( (D3DXQUATERNION*)pOut, (D3DXQUATERNION*)pQ ); 
}

inline unify::Quaternion* qQuaternionRotationAxis( unify::Quaternion* pOut, const unify::V3< unify::TFloat > * pV, unify::TFloat Angle )
{
	return (unify::Quaternion*)D3DXQuaternionRotationAxis( (D3DXQUATERNION*)pOut, (D3DXVECTOR3*)pV, Angle );
}

inline unify::Quaternion* qQuaternionRotationMatrix( unify::Quaternion* pOut, const TMatrix* pM )
{
	return (unify::Quaternion*)D3DXQuaternionRotationMatrix( (D3DXQUATERNION*)pOut, (D3DXMATRIX*)pM ); 
}

inline unify::Quaternion* qQuaternionRotationYawPitchRoll( unify::Quaternion* pOut, unify::TFloat Yaw, unify::TFloat Pitch, unify::TFloat Roll )
{
	return (unify::Quaternion*)D3DXQuaternionRotationYawPitchRoll( (D3DXQUATERNION*)pOut, Yaw, Pitch, Roll ); 
}

inline unify::Quaternion* qQuaternionSlerp( unify::Quaternion* pOut, const unify::Quaternion* pQ1, const unify::Quaternion* pQ2, unify::TFloat t )
{
	return (unify::Quaternion*)D3DXQuaternionSlerp( (D3DXQUATERNION*)pOut, (D3DXQUATERNION*)pQ1, (D3DXQUATERNION*)pQ2, t ); 
}

inline unify::Quaternion* qQuaternionSquad( unify::Quaternion* pOut, const unify::Quaternion* pQ1, const unify::Quaternion* pA, const unify::Quaternion* pB, const unify::Quaternion* pC, unify::TFloat t )
{
	return (unify::Quaternion*)D3DXQuaternionSquad( (D3DXQUATERNION*)pOut, (D3DXQUATERNION*)pQ1, (D3DXQUATERNION*)pA, (D3DXQUATERNION*)pB, (D3DXQUATERNION*)pC, t );
}

inline void qQuaternionSquadSetup( unify::Quaternion* pAOut, unify::Quaternion* pBOut, unify::Quaternion* pCOut, const unify::Quaternion* pQ0, const unify::Quaternion* pQ1, const unify::Quaternion* pQ2, const unify::Quaternion* pQ3 )
{
	D3DXQuaternionSquadSetup( (D3DXQUATERNION*)pAOut, (D3DXQUATERNION*)pBOut, (D3DXQUATERNION*)pCOut, (D3DXQUATERNION*)pQ0, (D3DXQUATERNION*)pQ1, (D3DXQUATERNION*)pQ2, (D3DXQUATERNION*)pQ3 ); 
}

inline void qQuaternionToAxisAngle( const unify::Quaternion* pQ, unify::V3< unify::TFloat > * pAxis, unify::TFloat* pAngle )
{
	D3DXQuaternionToAxisAngle( (D3DXQUATERNION*)pQ, (D3DXVECTOR3*)pAxis, (unify::TFloat*)pAngle ); 
}


////////////////////////////////////////////////////
// unify::Plane
////////////////////////////////////////////////////

inline unify::Plane* qPlaneFromPointNormal( unify::Plane* pOut, const unify::V3< unify::TFloat > * pPoint, const unify::V3< unify::TFloat > * pNormal )
{
	return (unify::Plane*)D3DXPlaneFromPointNormal( (D3DXPLANE*)pOut, (D3DXVECTOR3*)pPoint, (D3DXVECTOR3*)pNormal );
}

inline unify::Plane* qPlaneFromPoints( unify::Plane* pOut, const unify::V3< unify::TFloat > * pV1, const unify::V3< unify::TFloat > * pV2, const unify::V3< unify::TFloat > * pV3 )
{
	return (unify::Plane*)D3DXPlaneFromPoints( (D3DXPLANE*)pOut, (D3DXVECTOR3*)pV1, (D3DXVECTOR3*)pV2, (D3DXVECTOR3*)pV3 );
}

inline unify::V3< unify::TFloat > * qPlaneIntersectLine( unify::V3< unify::TFloat > * pOut, const unify::Plane* pP, const unify::V3< unify::TFloat > * pV1, const unify::V3< unify::TFloat > * pV2 )
{
	return (unify::V3< unify::TFloat > *)D3DXPlaneIntersectLine( (D3DXVECTOR3*)pOut, (D3DXPLANE*)pP, (D3DXVECTOR3*)pV1, (D3DXVECTOR3*)pV2 );
}

inline unify::Plane* qPlaneNormalize( unify::Plane* pOut, const unify::Plane* pP )
{
	return (unify::Plane*)D3DXPlaneNormalize( (D3DXPLANE*)pOut, (D3DXPLANE*)pP );
}

inline unify::Plane* qPlaneTransform( unify::Plane* pOut, const unify::Plane* pP, const TMatrix* pM )
{
	return (unify::Plane*)D3DXPlaneTransform( (D3DXPLANE*)pOut, (D3DXPLANE*)pP, (D3DXMATRIX*)pM );
}


///////////////////////////
// TMatrix
///////////////////////////

inline unify::V3< unify::TFloat > * TMatrix::GetLeft()
{
	return (unify::V3< unify::TFloat > *)mat.m[0];
}

inline unify::V3< unify::TFloat > * TMatrix::GetUp()
{
	return (unify::V3< unify::TFloat > *)mat.m[1];
}

inline unify::V3< unify::TFloat > * TMatrix::GetDir()
{
	return (unify::V3< unify::TFloat > *)mat.m[2];
}

inline unify::V3< unify::TFloat > * TMatrix::GetPos()
{
	return (unify::V3< unify::TFloat > *)mat.m[3];
}

inline unify::TFloat TMatrix::Determinant()
{
	return D3DXMatrixDeterminant( (D3DXMATRIX*)this );
}

inline unify::TBool TMatrix::IsIdentity()
{
	return D3DXMatrixIsIdentity( (D3DXMATRIX*)this ) == 0 ? false : true;
}

inline void TMatrix::Inverse()
{
	qMatrixInverse( this, NULL, this );
}

inline void TMatrix::Transpose()
{
	qMatrixTranspose( this, this );
}

inline TMatrix & TMatrix::operator = ( const TMatrix& matrix )
{
	memcpy( this, &matrix, sizeof( TMatrix ) );
	return *this;
}


// OPERATOR OVERLOADING MEMBER FUNCTIONS

inline TMatrix & TMatrix::operator += ( const TMatrix& mat )
{
	return( *(TMatrix*)&(TMatrix::mat += mat.mat) );
}

inline TMatrix & TMatrix::operator -= ( const TMatrix& mat )
{
	return( *(TMatrix*)&(TMatrix::mat -= mat.mat) );
}

inline TMatrix & TMatrix::operator *= ( const TMatrix& mat )
{
	return( *(TMatrix*)&(TMatrix::mat *= mat.mat) );
}

inline TMatrix & TMatrix::operator *= ( unify::TFloat val )
{
	return( *(TMatrix*)&(TMatrix::mat *= val) );
}

inline TMatrix & TMatrix::operator /= ( unify::TFloat val )
{
	return( *(TMatrix*)&(TMatrix::mat /= val) );
}


// binary operators
inline TMatrix TMatrix::operator + ( const TMatrix& mat ) const
{
	return( *(TMatrix*)&(TMatrix::mat * mat.mat) );
}

inline TMatrix TMatrix::operator - ( const TMatrix& mat ) const
{
	return( *(TMatrix*)&(TMatrix::mat - mat.mat) );
}

inline TMatrix TMatrix::operator * ( const TMatrix& mat ) const
{
	return( *(TMatrix*)&(TMatrix::mat * mat.mat) );
}

inline TMatrix TMatrix::operator * ( unify::TFloat val ) const
{
	return( *(TMatrix*)&(TMatrix::mat * val) );
}

inline TMatrix TMatrix::operator / ( unify::TFloat val ) const
{
	return( *(TMatrix*)&(TMatrix::mat / val) );
}

//friend D3DXVECTOR3 operator * ( unify::TFloat, CONST struct D3DXVECTOR3& );

inline unify::TBool TMatrix::operator == ( const TMatrix& mat ) const
{
	return (this->mat == mat.mat) == 0 ? false : true;
}

inline unify::TBool TMatrix::operator != ( const TMatrix& mat ) const
{
	return (this->mat != mat.mat) == 0 ? false : true;
}

inline unify::TFloat& TMatrix::operator()( unify::TUInt32 uRow, unify::TUInt32 uColumn )
{
	return mat(uRow, uColumn);
}

// Create an identity matrix...
inline TMatrix* qMatrixIdentity( TMatrix* pMatrix )
{
	D3DXMatrixIdentity( &pMatrix->mat );
	return pMatrix;
}

// Create a left-handed look-at matrix...
inline TMatrix* qMatrixLookAtLH( TMatrix* pOut, const unify::V3< unify::TFloat > * pEye, const unify::V3< unify::TFloat > * pAt, const unify::V3< unify::TFloat > * pUp )
{
	return (TMatrix*)D3DXMatrixLookAtLH( 
		(D3DXMATRIX*) pOut, (D3DXVECTOR3*) pEye,
		(D3DXVECTOR3*) pAt, (D3DXVECTOR3*) pUp );
}

// Create a right-handed look-at matrix...
inline TMatrix* qMatrixLookAtRH( TMatrix* pOut, const unify::V3< unify::TFloat > * pEye, const unify::V3< unify::TFloat > * pAt, const unify::V3< unify::TFloat > * pUp )
{
	return (TMatrix*)D3DXMatrixLookAtRH( 
		(D3DXMATRIX*) pOut, (D3DXVECTOR3*) pEye,
		(D3DXVECTOR3*) pAt, (D3DXVECTOR3*) pUp );
}

// Determines the product of two matrics...
inline TMatrix* qMatrixMultiply( TMatrix* pOut, const TMatrix* pM1, const TMatrix* pM2 )
{
	return (TMatrix*)D3DXMatrixMultiply( (D3DXMATRIX*)pOut, (D3DXMATRIX*)pM1, (D3DXMATRIX*)pM2 );
}

// Determines the product of two matrics, followed by a transpose...
inline TMatrix* qMatrixMultiplyTranspose( TMatrix* pOut, const TMatrix* pM1, const TMatrix* pM2 )
{
	return (TMatrix*)D3DXMatrixMultiplyTranspose( (D3DXMATRIX*)pOut, (D3DXMATRIX*)pM1, (D3DXMATRIX*)pM2 );
}

inline TMatrix* qMatrixOrthoLH( TMatrix* pOut, unify::TFloat w, unify::TFloat h, unify::TFloat zn, unify::TFloat zf )
{
	return (TMatrix*)D3DXMatrixOrthoLH( (D3DXMATRIX*) pOut, w, h, zn, zf );
}

inline TMatrix* qMatrixOrthoRH( TMatrix* pOut, unify::TFloat w, unify::TFloat h, unify::TFloat zn, unify::TFloat zf )
{
	return (TMatrix*)D3DXMatrixOrthoLH( (D3DXMATRIX*) pOut, w, h, zn, zf );
}

inline TMatrix* qMatrixOrthoOffCenterLH( TMatrix* pOut, unify::TFloat l, unify::TFloat r, unify::TFloat b, unify::TFloat t, unify::TFloat zn, unify::TFloat zf )
{
	return (TMatrix*)D3DXMatrixOrthoOffCenterLH( (D3DXMATRIX*) pOut, l, r, b, t, zn, zf );
}

inline TMatrix* qMatrixOrthoOffCenterRH( TMatrix* pOut, unify::TFloat l, unify::TFloat r, unify::TFloat b, unify::TFloat t, unify::TFloat zn, unify::TFloat zf )
{
	return (TMatrix*)D3DXMatrixOrthoOffCenterRH( (D3DXMATRIX*)pOut, l, r, b, t, zn, zf );
}

inline TMatrix* qMatrixInverse( TMatrix* pOut, unify::TFloat* pDeterminant, const TMatrix* pM )
{
	return (TMatrix*)D3DXMatrixInverse( (D3DXMATRIX*)pOut, pDeterminant, (D3DXMATRIX*) pM );
}

inline TMatrix* qMatrixAffineTransformation( TMatrix* pOut, unify::TFloat scaling, const unify::V3< unify::TFloat > * pRotationCenter, const unify::Quaternion* pRotation, const unify::V3< unify::TFloat > * pTranslation )
{
	return (TMatrix*)D3DXMatrixAffineTransformation( (D3DXMATRIX*)pOut, scaling, (D3DXVECTOR3*)pRotationCenter, (D3DXQUATERNION*)pRotation, (D3DXVECTOR3*)pTranslation );
}

inline TMatrix* qMatrixPerspectiveFovLH( TMatrix* pOut, unify::TFloat fovy, unify::TFloat Aspect, unify::TFloat zn, unify::TFloat zf )
{
	return (TMatrix*) D3DXMatrixPerspectiveFovLH( (D3DXMATRIX*) pOut, fovy, Aspect, zn, zf );
}

inline TMatrix* qMatrixPerspectiveFovRH( TMatrix* pOut, unify::TFloat fovy, unify::TFloat Aspect, unify::TFloat zn, unify::TFloat zf )
{
	return (TMatrix*) D3DXMatrixPerspectiveFovRH( (D3DXMATRIX*) pOut, fovy, Aspect, zn, zf );
}

inline TMatrix* qMatrixPerspectiveLH( TMatrix* pOut, unify::TFloat w, unify::TFloat h, unify::TFloat zn, unify::TFloat zf )
{
	return (TMatrix*) D3DXMatrixPerspectiveLH( (D3DXMATRIX*) pOut, w, h, zn, zf );
}

inline TMatrix* qMatrixPerspectiveRH( TMatrix* pOut, unify::TFloat w, unify::TFloat h, unify::TFloat zn, unify::TFloat zf )
{
	return (TMatrix*) D3DXMatrixPerspectiveRH( (D3DXMATRIX*) pOut, w, h, zn, zf );
}

inline TMatrix* qMatrixPerspectiveOffCenterLH( TMatrix* pOut, unify::TFloat l, unify::TFloat r, unify::TFloat b, unify::TFloat t, unify::TFloat zn, unify::TFloat zf )
{
	return (TMatrix*)D3DXMatrixPerspectiveOffCenterLH( (D3DXMATRIX*) pOut, l, r, b, t, zn, zf );
} 

inline TMatrix* qMatrixPerspectiveOffCenterRH( TMatrix* pOut, unify::TFloat l, unify::TFloat r, unify::TFloat b, unify::TFloat t, unify::TFloat zn, unify::TFloat zf )
{
	return (TMatrix*)D3DXMatrixPerspectiveOffCenterRH( (D3DXMATRIX*)pOut, l, r, b, t, zn, zf );
} 

inline TMatrix* qMatrixReflect( TMatrix* pOut, const unify::Plane* pPlane )
{
	return (TMatrix*)D3DXMatrixReflect( (D3DXMATRIX*)pOut, (D3DXPLANE*)pPlane );
}

// Create a rotation matrix about a given axis and of a given offset angle...
inline TMatrix* qMatrixRotationAxis( TMatrix* pMatrix, const unify::V3< unify::TFloat > * vAxis, unify::TFloat fAngle )
{
	D3DXMatrixRotationAxis( &pMatrix->mat, (D3DXVECTOR3*)vAxis, fAngle );
	return pMatrix;
}

inline TMatrix* qMatrixRotationQuaternion( TMatrix* pOut, const unify::Quaternion* pQ )
{
	return (TMatrix*)D3DXMatrixRotationQuaternion( (D3DXMATRIX*)pOut, (D3DXQUATERNION*)pQ );
}

inline TMatrix* qMatrixRotationX( TMatrix* pOut, unify::TFloat Angle )
{
	return (TMatrix*)D3DXMatrixRotationX( (D3DXMATRIX*)pOut, Angle );
}

inline TMatrix* qMatrixRotationY( TMatrix* pOut, unify::TFloat Angle )
{
	return (TMatrix*)D3DXMatrixRotationY( (D3DXMATRIX*)pOut, Angle );
}

inline TMatrix* qMatrixRotationZ( TMatrix* pOut, unify::TFloat Angle )
{
	return (TMatrix*)D3DXMatrixRotationZ( (D3DXMATRIX*)pOut, Angle );
}

inline TMatrix* qMatrixRotationYawPitchRoll( TMatrix* pOut, unify::TFloat Yaw, unify::TFloat Pitch, unify::TFloat Roll )
{
	return (TMatrix*)D3DXMatrixRotationYawPitchRoll( (D3DXMATRIX*)pOut, Yaw, Pitch, Roll ); 
}

inline TMatrix* qMatrixScaling( TMatrix* pOut, unify::TFloat sx, unify::TFloat sy, unify::TFloat sz )
{
	return (TMatrix*)D3DXMatrixScaling( (D3DXMATRIX*)pOut, sx, sy, sz ); 
}

inline TMatrix* qMatrixShadow( TMatrix* pOut, const unify::V4< unify::TFloat >* pLight, const unify::Plane* pPlane )
{
	return (TMatrix*)D3DXMatrixShadow( (D3DXMATRIX*)pOut, (D3DXVECTOR4*)pLight, (D3DXPLANE*)pPlane );
}

inline TMatrix* qMatrixTransformation( TMatrix* pOut, const unify::V3< unify::TFloat > * pScalingCenter, const unify::Quaternion* pScalingRotation,
									const unify::V3< unify::TFloat > * pScaling, const unify::V3< unify::TFloat > * pRotationCenter, const unify::Quaternion* pRotation, const unify::V3< unify::TFloat > * pTranslation )
{
	return (TMatrix*)D3DXMatrixTransformation( (D3DXMATRIX*)pOut, (D3DXVECTOR3*)pScalingCenter, (D3DXQUATERNION*)pScalingRotation, (D3DXVECTOR3*)pScaling, (D3DXVECTOR3*)pRotationCenter, (D3DXQUATERNION*)pRotation, (D3DXVECTOR3*)pTranslation );
}

inline TMatrix* qMatrixTranslation( TMatrix* pOut, unify::TFloat x, unify::TFloat y, unify::TFloat z ) 
{
	return (TMatrix*)D3DXMatrixTranslation( (D3DXMATRIX*)pOut, x, y, z ); 
}

inline TMatrix* qMatrixTranslation( TMatrix* pOut, const unify::V3< unify::TFloat > * pVec ) 
{
	return (TMatrix*)D3DXMatrixTranslation( (D3DXMATRIX*)pOut, pVec->x, pVec->y, pVec->z ); 
}

inline TMatrix* qMatrixTranspose( TMatrix* pOut, const TMatrix* pM )
{
	return (TMatrix*)D3DXMatrixTranspose( (D3DXMATRIX*) pOut, (D3DXMATRIX*) pM );
}


///////////////////////////////////
// TMatrixIdentity
///////////////////////////////////

inline TMatrixIdentity::TMatrixIdentity()
{
	//qMatrixIdentity( (TMatrix*)&mat );
	qMatrixIdentity( this );
}

	
///////////////////////////////////
// unify::Plane
///////////////////////////////////

inline unify::TFloat qTPlaneDotCoord( const unify::Plane & plane, const unify::V3< unify::TFloat > * pV )
{
	return D3DXPlaneDotCoord( (D3DXPLANE*)&plane, (D3DXVECTOR3*)pV );
}

inline unify::TFloat qTPlaneDotNormal( const unify::Plane & plane, const unify::V3< unify::TFloat > * pV )
{
	return D3DXPlaneDotNormal( (D3DXPLANE*)&plane, (D3DXVECTOR3*)pV );
}

///////////////////////////////////
// TRay
///////////////////////////////////

inline unify::TBool RayIntersectsBBox( const unify::BBox< unify::TFloat > * pBBox, const unify::Ray< unify::V3< unify::TFloat > > * pRay )
{
	return D3DXBoxBoundProbe( (LPD3DXVECTOR3)&pBBox->inf, (LPD3DXVECTOR3)&pBBox->sup, (LPD3DXVECTOR3)&pRay->pos, (LPD3DXVECTOR3)&pRay->dir ) == 0 ? false : true;
}
