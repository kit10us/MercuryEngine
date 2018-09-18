// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <me/render/MeshInstanceData.h>

using namespace me;
using namespace render;

MeshInstanceData::MeshInstanceData( const unify::FrameSet & frameSet, const frameanimation::AnimationSet & animationSet )
: m_playing( false )
, m_animationSet( &animationSet )
, m_frameSetInstance( &frameSet )
{
}

MeshInstanceData::~MeshInstanceData()
{
}

void MeshInstanceData::PlaySingleAnimation( std::string name, bool forceLoop )
{
	m_activeAnimations.clear();
	PlayAdditionalAnimation( name, forceLoop );
}

void MeshInstanceData::PlayAdditionalAnimation( std::string name, bool forceLoop )
{
	frameanimation::Animation::const_ptr animation = m_animationSet->FindAnimation( name );
	m_activeAnimations.push_back( ActiveAnimation( unify::TimeDelta(), animation, forceLoop ) );
	m_playing = true;
}

void MeshInstanceData::PauseAnimations( bool pause )
{
	m_playing = false;
}

bool MeshInstanceData::Paused() const
{
	return ! m_playing;
}

size_t MeshInstanceData::AnimationCount() const
{
	return m_activeAnimations.size();
}

void MeshInstanceData::RemoveAnimation( std::string name )
{
	for( std::list< ActiveAnimation >::iterator itr = m_activeAnimations.begin(), end = m_activeAnimations.end(); itr != end; ++itr )
	{
		if( unify::StringIs( itr->m_animation.get()->Name(), name ) )
		{
			m_activeAnimations.erase( itr );
			break;
		}
	}
}

void MeshInstanceData::RemoveAllAnimations()
{
	m_activeAnimations.clear();
}

void MeshInstanceData::Update( const RenderInfo & renderInfo )
{
	if( ! Paused() )
	{
		for( std::list< ActiveAnimation >::iterator itr = m_activeAnimations.begin(), end = m_activeAnimations.end(); itr != end; ++itr )
		{
			auto & progress = itr->m_time;
			frameanimation::Animation::const_ptr animation = itr->m_animation;
			progress += renderInfo.GetDelta();
			if( progress >= animation->Duration() )
			{
				if( animation->Loops() || itr->m_forceLoop )
				{
					float missedCycles = 0;
					while( progress > animation->Duration() )
					{
						progress -= animation->Duration();
						missedCycles++;
					}
				}
				else
				{
					progress = animation->Duration();
				}
			}
		}
	}

	SyncFrames();
}

void MeshInstanceData::RenderFrames( const unify::Matrix & world, bool major, bool minor )
{
	 world; major; minor ;
}

unify::FrameSetInstance & MeshInstanceData::GetFrameSetInstance()
{
	return m_frameSetInstance;
}

void MeshInstanceData::SyncFrames()
{
	m_frameSetInstance.Reset();
	for( std::list< ActiveAnimation >::iterator itr = m_activeAnimations.begin(), end = m_activeAnimations.end(); itr != end; ++itr )
	{
		auto progress = itr->m_time;
		frameanimation::Animation::const_ptr animation = itr->m_animation;

		animation->ApplyToFrames( progress, m_frameSetInstance );
	}
	m_frameSetInstance.UpdateLocals();
}