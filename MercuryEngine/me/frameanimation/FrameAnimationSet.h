// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/frameanimation/FrameAnimation.h>

namespace me
{
	namespace frameanimation
	{
		class AnimationSet
		{
		public:
			typedef std::shared_ptr< AnimationSet > ptr;

			AnimationSet();
			~AnimationSet();
			void AddAnimation( Animation::ptr animation );
			Animation::ptr FindAnimation( const std::string & name );
			const Animation::ptr FindAnimation( const std::string & name ) const;

		private:
			typedef std::string AnimationName;
			std::map< AnimationName, Animation::ptr > m_animations;
		};
	}
}