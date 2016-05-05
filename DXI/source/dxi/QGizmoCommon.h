/*
 * Copyright (c) 2004 Quentin S. Smith
 * All Rights Reserved
 *
 *	Common gizmos:
 *		Gizmos are objects that preform a certain function - this can varry from assigning values, scaling values,
 *	transitions, anything. They are standardized with inheritance to allow collections and lists for animations and
 *	events.
 */
#pragma once

#include "dxi/QGizmo.h"

namespace dxi
{

	/*
	// A gizmo that changes a value from START to FINISH over LENGTH...
	class QGizmoLinear : public QGizmo
	{
		GIZMO_TYPE( "Linear" );
	public:
		QGizmoLinear();
		bool Update( unify::Seconds fTime );
		void SetStart( float fStart );
		void SetFinish( float fFinish );
		void SetLength( float fLength );

	protected:
		float* m_pValue;
		float m_fStart;
		float m_fFinish;
		float m_fLength;
	};
	*/


	// Set position to another position PosA = PosB 
	class QGizmoLinkPosition : public QGizmo
	{
		GIZMO_TYPE( "LinkPosition" );

	public:
		QGizmoLinkPosition();
		void Update( unify::Seconds time );
		void SetPosition( unify::V3< float > * pPos );
		void SetLinkPosition( unify::V3< float > * pPos );

	protected:
		unify::V3< float > * m_pPosA;
		unify::V3< float > * m_pPosB;
	};


	// Set position to (Set A B or C)
	class QGizmoSetPosition : public QGizmo
	{
		GIZMO_TYPE( "SetPosition" );

	public:
		QGizmoSetPosition();
		void Update( unify::Seconds time );
		void SetPosition( unify::V3< float > * pPos );
		void SetX( float fX );
		void SetY( float fY );
		void SetZ( float fZ );
		void Unset( bool bX, bool bY, bool bZ );

	protected:
		unify::V3< float > * m_pPos;
		unify::V3< float > m_vPos;

		bool m_bSetX;
		bool m_bSetY;
		bool m_bSetZ;
	};


	class QGizmoPositionController : public QGizmo
	{
		GIZMO_TYPE( "PositionController" );

	public:
		void Update( unify::Seconds time );
		void SetPosition( unify::V3< float > * pPos );
		QGizmoSetPosition* GetGizmoSet();
		QGizmoLinkPosition* GetGizmoLink();

	protected:
		QGizmoSetPosition m_GizmoSet;
		QGizmoLinkPosition m_GizmoLink;
	};
		
	#include "dxi/QGizmoCommon.inl"
}