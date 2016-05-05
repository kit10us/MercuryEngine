#pragma once

#include <dxi/frameanimation/FrameAnimation.h>

namespace dxi
{
	namespace frameanimation
	{
		class AnimationSet
		{
		public:
			AnimationSet();
			~AnimationSet();
			void AddAnimation( Animation::shared_ptr animation );
			Animation::shared_ptr FindAnimation( const std::string & name );
			const Animation::shared_ptr FindAnimation( const std::string & name ) const;

		private:
			std::map< std::string /*animation name*/, std::shared_ptr< Animation > > m_animations;
		};
	}
}