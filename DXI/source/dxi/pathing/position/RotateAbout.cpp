// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/pathing/position/RotateAbout.h>

using namespace dxi;
using namespace pathing;
using namespace position;

RotateAbout::RotateAbout( const unify::V3< float > * position, const float distance, const unify::Quaternion * axisFrom, const unify::Quaternion * axisTo )
: m_position( position )
, m_distance( distance )
, m_axisFrom( axisFrom )
, m_axisTo( axisTo )
{
}

void RotateAbout::AtTime( float unitTime, unify::Frame * frame )
{
	unify::Quaternion intermediate( unify::QuaternionSlerp( *m_axisFrom, *m_axisTo, unitTime ) );
	unify::V3< float > position( 0, 0, 1 );
	intermediate.TransformVector( position );
	position *= m_distance;
	position += *m_position;
	frame->SetPosition( position );
}
