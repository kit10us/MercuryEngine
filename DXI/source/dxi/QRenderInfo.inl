inline unify::Bool QRenderInfo::CheckFlag( RenderInfoFlag flag )
{
	if( this ) return CHECKFLAG( m_uRenderFlags, flag );
	return FALSE;
}

inline unify::Bool QRenderInfo::CheckEffect( void* pNewEffect )
{
	if( this ) return CHECKFLAG( m_uRenderFlags, Render_RetainEffect ) ? ( m_pEffect == pNewEffect ? FALSE : TRUE ) : TRUE;
	return FALSE;
}

inline void QRenderInfo::SetEffect( void * pEffect )
{
	m_pEffect = pEffect;
}

inline void * QRenderInfo::GetEffect()
{
	return m_pEffect;
}

inline unify::Float QRenderInfo::DistanceFromCamera() const
{
	return m_fDistanceFromCamera;
}

inline void QRenderInfo::SetDistanceFromCamera( unify::Float fDistance )
{
	m_fDistanceFromCamera = fDistance;
}

inline void QRenderInfo::SetRenderFlags( unify::UInt32 uFlags )
{
	m_uRenderFlags = uFlags;
}

/*
inline void QRenderInfo::SetOnlyThisTexture( QTexture * pTexture )
{
	m_pOnlyWithThisTexture = pTexture;
}
*/