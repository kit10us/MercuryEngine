// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/pathing/PathController.h>

using namespace dxi;
using namespace pathing;

PathController::PathController( Path::shared_ptr path, unify::Frame * frame, OnDone::shared_ptr onDone )
: m_frame( frame ), m_path( path ), m_onDone( onDone ), m_looping( false )
, m_active( true ), m_totalElapsed( 0 )
{
}

PathController::~PathController()
{
}

void PathController::SetFrame( unify::Frame * frame )
{
	m_frame = frame;
}

void PathController::SetPath( Path::shared_ptr path )
{
	m_path = path;
}

void PathController::SetOnDone( OnDone::shared_ptr onDone )
{
	m_onDone = onDone;
}

void PathController::SetLooping( bool looping )
{
	m_looping = looping;
}

bool PathController::GetLooping() const
{
	return m_looping;
}

void PathController::Update( unify::Seconds elapsed )
{
	if( m_path && m_active )
	{
		m_totalElapsed += elapsed;
		float excess = m_path->Update( m_totalElapsed, m_frame );
		if( excess ) // Then we are done.
		{
			if( m_looping )
			{
				m_totalElapsed = 0;
				Update( excess );
			}
			else
			{
				m_active = false;
				m_totalElapsed -= excess;
				if( m_onDone )
				{
					(*m_onDone)( this, excess );
				}
			}
		}
	}
}

void PathController::Restart( unify::Seconds elapsed )
{
	m_totalElapsed = 0;
	m_active = true;
	Update( elapsed );
}

void PathController::Pause()
{
	m_active = false;
}

void PathController::Resume()
{
	m_active = true;
}