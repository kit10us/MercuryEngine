// Copyright (c) 2002 - 2018, Kit10 Studios LLC
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

Animation::ptr AnimationSet::FindAnimation( std::string name )
{
	std::map< std::string, Animation::ptr >::iterator itr = m_animations.find( name );
	if ( itr == m_animations.end() )
	{
		return Animation::ptr();
	}
	return itr->second;
}

const Animation::ptr AnimationSet::FindAnimation( std::string name ) const
{
	std::map< std::string, Animation::ptr >::const_iterator itr = m_animations.find( name );
	if ( itr == m_animations.end() )
	{
		return Animation::ptr();
	}
	return itr->second;
}

