// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/animation/Instance.h>
#include <dxi/animation/Sequence.h>

using namespace dxi;
using namespace animation;

Instance::Instance()
: m_currentTime( 0 )
{
}

Instance::Instance( const Instance & instance )
: m_currentTime( instance.m_currentTime )
, m_sequence( instance.m_sequence )
, m_frame( instance.m_frame )
{
}

Instance::Instance( const Frame & frame )
: m_currentTime( 0 )
, m_frame( frame )
{
}

Instance::Instance( std::shared_ptr< Sequence > sequence, unify::Seconds time )
: m_currentTime( time )
, m_sequence( sequence )
, m_frame( sequence->GetFrameByTime( time ) )
{
}

Instance::~Instance()
{
}

bool Instance::Update( unify::Seconds elapsed )
{
	bool changed = false;
	if( m_sequence )
	{
		m_currentTime += elapsed;
		Frame newFrame( m_sequence->GetFrameByTime( m_currentTime ) );
		if( newFrame != m_frame )
		{
			m_frame = newFrame;
			changed = true;
		}
	}
	else
	{
		// Do nothing.
	}
	return changed;
}

const Frame & Instance::GetFrame() const
{
	return m_frame;
}
