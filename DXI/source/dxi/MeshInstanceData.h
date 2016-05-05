// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/Geometry.h>
#include <dxi/frameanimation/FrameAnimationSet.h>
#include <unify/FrameSet.h>

namespace dxi
{
	class MeshAnimatorInterface
	{
	public:
		virtual ~MeshAnimatorInterface() {}

		// Will remove any existing animations.
		virtual void PlaySingleAnimation( const std::string & name, bool forceLoop = false ) = 0;

		// Will NOT remove existing animations.
		virtual void PlayAdditionalAnimation( const std::string & name, bool forceLoop = false ) = 0;

		virtual void PauseAnimations( bool pause ) = 0;
		virtual bool Paused() const = 0;
		virtual size_t AnimationCount() const = 0;

		virtual void RemoveAnimation( const std::string & name ) = 0;
		virtual void RemoveAllAnimations() = 0;
	};

	class MeshInstanceData : public GeometryInstanceData, public MeshAnimatorInterface
	{
	public:
		MeshInstanceData( const unify::FrameSet & frameSet, const frameanimation::AnimationSet & animationSet );
		~MeshInstanceData();

		// Supporting MeshAnimatorInterface...
		void PlaySingleAnimation( const std::string & name, bool forceLoop = false );
		void PlayAdditionalAnimation( const std::string & name, bool forceLoop = false );
		void PauseAnimations( bool pause );
		bool Paused() const;
		size_t AnimationCount() const;
		void RemoveAnimation( const std::string & name );
		void RemoveAllAnimations();

		void Update( unify::Seconds elapsed );
		void RenderFrames( const unify::Matrix & world, bool major, bool minor );

		unify::FrameSetInstance & GetFrameSetInstance();

	private:
		void SyncFrames();

		bool m_playing;
		const frameanimation::AnimationSet * m_animationSet;
		
		struct ActiveAnimation
		{
			ActiveAnimation( unify::Seconds time, frameanimation::Animation::const_shared_ptr animation, bool forceLoop )
				: m_time( time ), m_animation( animation ), m_forceLoop( forceLoop )
			{
			}

			unify::Seconds m_time;
			frameanimation::Animation::const_shared_ptr m_animation;
			bool m_forceLoop;
		};

		std::list< ActiveAnimation > m_activeAnimations;
		unify::FrameSetInstance m_frameSetInstance;
	};
}