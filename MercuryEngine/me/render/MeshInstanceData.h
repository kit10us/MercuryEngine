// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/render/Geometry.h>
#include <me/frameanimation/FrameAnimationSet.h>
#include <unify/FrameSetInstance.h>

namespace me
{
	namespace render
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

			void Update( const RenderInfo & renderInfo );
			void RenderFrames( const unify::Matrix & world, bool major, bool minor );

			unify::FrameSetInstance & GetFrameSetInstance();

		private:
			void SyncFrames();

			bool m_playing;
			const me::frameanimation::AnimationSet * m_animationSet;

			struct ActiveAnimation
			{
				ActiveAnimation( unify::Seconds time, me::frameanimation::Animation::const_ptr animation, bool forceLoop )
					: m_time( time ), m_animation( animation ), m_forceLoop( forceLoop )
				{
				}

				unify::Seconds m_time;
				me::frameanimation::Animation::const_ptr m_animation;
				bool m_forceLoop;
			};

			std::list< ActiveAnimation > m_activeAnimations;
			unify::FrameSetInstance m_frameSetInstance;
		};
	}
}