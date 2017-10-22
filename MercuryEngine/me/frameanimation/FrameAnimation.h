// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/frameanimation/FrameAnimationKey.h>
#include <unify/FrameSetInstance.h>
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
			typedef std::shared_ptr< Animation > ptr;
			typedef std::shared_ptr< const Animation > const_ptr;

			Animation( std::string name, unify::Seconds duration );
			~Animation();

			std::string Name() const;
			unify::Seconds Duration() const;
			bool Loops() const;
			void AddScaleKey( size_t boneIndex, const ScaleKey & key );
			void AddRotationKey( size_t boneIndex, const RotationKey & key );
			void AddTranslationKey( size_t BoneIndex, const TranslationKey & key );
			
			/// <summary>
			/// Apply to frames, returning the actual elapsed time clamped to our timeline.
			/// </summary>
			float ApplyToFrames( float elapsedTime, unify::FrameSetInstance & frameSetInstance ) const;

		private:
			std::string m_name;
			bool m_loops;
			unify::Seconds m_duration;
			typedef size_t TargetFrameIndex;
			std::map< TargetFrameIndex, SRTTimeline > m_boneAnimations;
		};
	}
}