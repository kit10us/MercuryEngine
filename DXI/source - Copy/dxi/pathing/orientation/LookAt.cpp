// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/pathing/orientation/LookAt.h>

using namespace dxi;
using namespace pathing;
using namespace orientation;

LookAt::LookAt( const unify::V3< float > * position )
: m_position( position )
, m_frame( 0 )
{
}

LookAt::LookAt( const unify::Frame * frame )
: m_position( 0 )
, m_frame( frame )
{
}

void LookAt::AtTime( float unitTime, unify::Frame * frame )
{
	unitTime; // Ignore.
	if( m_frame )
	{
		frame->LookAt( m_frame->GetPosition() );
	}
	else
	{
		frame->LookAt( *m_position );
	}
}
