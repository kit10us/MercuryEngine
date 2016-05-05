// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/pathing/position/AttachTo.h>

using namespace dxi;
using namespace pathing;
using namespace position;

AttachTo::AttachTo( const unify::Frame * targetFrame, float distance, const unify::Quaternion & axisFromTarget )
: m_targetFrame( targetFrame )
, m_targetPosition( 0 )
, m_distance( distance )
, m_axisFromTarget( axisFromTarget )
{
}

AttachTo::AttachTo( const unify::V3< float > * targetPosition, float distance, const unify::Quaternion & axisFromTarget )
: m_targetFrame( 0 )
, m_targetPosition( targetPosition )
, m_distance( distance )
, m_axisFromTarget( axisFromTarget )
{
}

void AttachTo::AtTime( float unitTime, unify::Frame * frame )
{
	if( m_targetFrame )
	{
		unify::Quaternion orientation( frame->GetFinalMatrix().GetRotation() );
		orientation *= m_axisFromTarget;
		unify::Matrix matrix( orientation );
		unify::V3< float > position( 0, 0, 1 );
		matrix.TransformNormal( position );
		position *= m_distance;
		position += frame->GetFinalMatrix().GetPosition();
		frame->SetPosition( position );
	}
	else if( m_targetPosition )
	{
		unify::Matrix matrix( m_axisFromTarget );
		unify::V3< float > position( 0, 0, 1 );
		matrix.TransformNormal( position );
		position *= m_distance;
		position += frame->GetFinalMatrix().GetPosition();
		frame->SetPosition( position );
	}
}
