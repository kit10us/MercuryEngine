// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/frameanimation/FrameAnimationKey.h>
#include <unify/FrameSet.h>
#include <memory>
#include <unify/TimeDelta.h>

namespace me
{
	namespace frameanimation
	{
		// An animation. Apply this to a frame to animate the frame.
		class Animation
		{
		public:
			typedef std::shared_ptr< Animation > shared_ptr;
			typedef std::shared_ptr< const Animation > const_shared_ptr;

			Animation( const std::string & name, unify::Seconds duration );
			~Animation();

			const std::string & Name() const;
			unify::Seconds Duration() const;
			bool Loops() const;
			void AddScaleKey( size_t boneIndex, const ScaleKey & key );
			void AddRotationKey( size_t boneIndex, const RotationKey & key );
			void AddTranslationKey( size_t BoneIndex, const TranslationKey & key );
			void ApplyToFrames( float progress, unify::FrameSetInstance & frameSetInstance ) const;

		private:
			std::string m_name;
			bool m_loops;
			unify::Seconds m_duration;
			std::map< size_t /*target frame*/, SRTTimeline > m_boneAnimations;
		};
	}
}