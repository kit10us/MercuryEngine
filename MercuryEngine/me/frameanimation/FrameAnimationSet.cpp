#include <me/frameanimation/FrameAnimationSet.h>

using namespace me;
using namespace frameanimation;

AnimationSet::AnimationSet()
{
}

AnimationSet::~AnimationSet()
{
}

void AnimationSet::AddAnimation( Animation::shared_ptr animation )
{
	m_animations.insert( std::pair< std::string, Animation::shared_ptr >( animation->Name(), animation ) );
}

Animation::shared_ptr AnimationSet::FindAnimation( const std::string & name )
{
	std::map< std::string, Animation::shared_ptr >::iterator itr = m_animations.find( name );
	if ( itr == m_animations.end() )
	{
		return Animation::shared_ptr();
	}
	return itr->second;
}

const Animation::shared_ptr AnimationSet::FindAnimation( const std::string & name ) const
{
	std::map< std::string, Animation::shared_ptr >::const_iterator itr = m_animations.find( name );
	if ( itr == m_animations.end() )
	{
		return Animation::shared_ptr();
	}
	return itr->second;
}

