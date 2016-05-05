////////////////////////////////////
// QGizmoLinkPosition
////////////////////////////////////

inline QGizmoLinkPosition::QGizmoLinkPosition()
{
	m_pPosA = 0;
	m_pPosB = 0;
}

inline void QGizmoLinkPosition::Update( unify::Seconds fTime )
{
	if( ! m_pPosA || ! m_pPosB ) return;
	*m_pPosA = *m_pPosB;
}

inline void QGizmoLinkPosition::SetPosition( unify::V3< float > * pPos )
{
	m_pPosA = pPos;
}

inline void QGizmoLinkPosition::SetLinkPosition( unify::V3< float > * pPos )
{
	m_pPosB = pPos;
}


////////////////////////////////////
// QGizmoSetPosition
////////////////////////////////////

inline QGizmoSetPosition::QGizmoSetPosition()
{
	m_pPos = 0;
	m_bSetX = FALSE;
	m_bSetY = FALSE;
	m_bSetZ = FALSE;
}

inline void QGizmoSetPosition::SetPosition( unify::V3< float > * pPos )
{
	m_pPos = pPos;
}

inline void QGizmoSetPosition::Update( unify::Seconds fTime )
{
	if( ! m_pPos ) return;
	if( m_bSetX ) m_pPos->x = m_vPos.x;
	if( m_bSetY ) m_pPos->y = m_vPos.y;
	if( m_bSetZ ) m_pPos->z = m_vPos.z;
}

inline void QGizmoSetPosition::SetX( float fX )
{
	m_bSetX = true;
	m_vPos.x = fX;
}

inline void QGizmoSetPosition::SetY( float fY )
{
	m_bSetY = true;
	m_vPos.y = fY;
}

inline void QGizmoSetPosition::SetZ( float fZ )
{
	m_bSetZ = true;
	m_vPos.z = fZ;
}

inline void QGizmoSetPosition::Unset( bool bX, bool bY, bool bZ )
{
	m_bSetX = bX;
	m_bSetY = bY;
	m_bSetZ = bZ;
}


////////////////////////////////////
// QGizmoPositionController
////////////////////////////////////

inline void QGizmoPositionController::Update( unify::Seconds fTime )
{
	m_GizmoLink.Update( fTime );
	m_GizmoSet.Update( fTime );
}

inline void QGizmoPositionController::SetPosition( unify::V3< float > * pPos )
{
	m_GizmoSet.SetPosition( pPos );
	m_GizmoLink.SetPosition( pPos );
}

inline QGizmoSetPosition * QGizmoPositionController::GetGizmoSet()
{
	return &m_GizmoSet;
}

inline QGizmoLinkPosition* QGizmoPositionController::GetGizmoLink()
{
	return &m_GizmoLink;
}

