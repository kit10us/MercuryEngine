// Copyright (c) 2002 - 2018, Evil Quail LLC
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

			Animation( std::string name, unify::TimeDelta duration );
			~Animation();

			std::string Name() const;
			unify::TimeDelta Duration() const;
			bool Loops() const;
			void AddScaleKey( size_t boneIndex, const ScaleKey & key );
			void AddRotationKey( size_t boneIndex, const RotationKey & key );
			void AddTranslationKey( size_t BoneIndex, const TranslationKey & key );
			
			/// <summary>
			/// Apply to frames, returning the actual elapsed time clamped to our timeline.
			/// </summary>
			unify::TimeDelta ApplyToFrames( unify::TimeDelta elapsedTime, unify::FrameSetInstance & frameSetInstance ) const;

		private:
			std::string m_name;
			bool m_loops;
			unify::TimeDelta m_duration;
			typedef size_t TargetFrameIndex;
			std::map< TargetFrameIndex, SRTTimeline > m_boneAnimations;
		};
	}
}