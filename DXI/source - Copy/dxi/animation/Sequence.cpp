// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/animation/Group.h>

using namespace dxi;
using namespace animation;

Sequence::Sequence( Group * parent, bool loops )
: m_parent( parent )
, m_totalDuration( 0 )
, m_loops( loops )
{
}

Sequence::~Sequence()
{
	// We do not own m_parent.
	m_parent = 0;
}

void Sequence::Add( const Frame & frame )
{
	m_frames.push_back( frame );
	m_totalDuration += frame.GetDuration();
}

const Frame & Sequence::GetBegin() const
{
	return m_frames[ 0 ];
}

const Frame & Sequence::GetFrameByIndex( FrameIndex index ) const
{
	return m_frames[ index ];
}

const Frame & Sequence::GetFrameByTime( unify::Seconds & time ) const
{
	// Return the last frame if we are past the end of the sequence and we don't repeat.
	if( m_totalDuration == 0 || (m_loops == false && time >= m_totalDuration) )
	{
		time = m_totalDuration;
		return m_frames.back();
	}

	// Clamp our time to animation.
	while( time < 0 ) time += m_totalDuration;
	while( time > m_totalDuration ) time -= m_totalDuration;

	unsigned int index = 0;
	unify::Seconds frameTime = 0;
	for( FrameList::const_iterator itr = m_frames.begin(); itr != m_frames.end(); ++itr, ++index )
	{
		if( time <= (frameTime += itr->GetDuration()) )
		{
			break;
		}
	}
	return m_frames[ index ];
}

unsigned int Sequence::FrameCount() const
{
	return static_cast< unsigned int >( m_frames.size() );
}

unify::Seconds Sequence::Duration() const
{
	return m_totalDuration;
}

void Sequence::SetLoops( bool loops )
{
	m_loops = loops;
}

bool Sequence::Loops() const
{
	return m_loops;
}
