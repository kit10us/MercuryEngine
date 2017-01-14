// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/frameanimation/FrameAnimationSet.h>

using namespace me;
using namespace frameanimation;

AnimationSet::AnimationSet()
{
}

AnimationSet::~AnimationSet()
{
}

void AnimationSet::AddAnimation( Animation::ptr animation )
{
	m_animations.insert( std::pair< std::string, Animation::ptr >( animation->Name(), animation ) );
}

Animation::ptr AnimationSet::FindAnimation( const std::string & name )
{
	std::map< std::string, Animation::ptr >::iterator itr = m_animations.find( name );
	if ( itr == m_animations.end() )
	{
		return Animation::ptr();
	}
	return itr->second;
}

const Animation::ptr AnimationSet::FindAnimation( const std::string & name ) const
{
	std::map< std::string, Animation::ptr >::const_iterator itr = m_animations.find( name );
	if ( itr == m_animations.end() )
	{
		return Animation::ptr();
	}
	return itr->second;
}

