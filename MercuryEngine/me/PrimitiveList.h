// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/BufferSet.h>
#include <me/RenderParams.h>
#include <me/frameanimation/FrameAnimationSet.h>
#include <unify/BBox.h>
#include <unify/BSphere.h>
#include <unify/FrameLite.h>

namespace me
{
	///<summary>
	/// Holds data on how to render a set of primitives.
	///</summary>
	class PrimitiveList
	{
	public:
		PrimitiveList( me::IRenderer * renderer );
		~PrimitiveList();

		PrimitiveList( const PrimitiveList & ) = delete;

		void Destroy();

		void Render( RenderParams params, render::MatrixFeed & matrixFeed ) const;
		
		BufferSet & AddBufferSet();
		void RemoveBufferSet( size_t i );
		void RemoveAllBufferSets();

		size_t GetBufferSetCount() const;
		BufferSet & GetBufferSet( size_t index );
		const BufferSet & GetBufferSet( size_t index ) const;

		unify::FrameSet & GetFrameSet();
		const unify::FrameSet & GetFrameSet() const;

		frameanimation::AnimationSet & GetAnimationSet();
		const frameanimation::AnimationSet & GetAnimationSet() const;

		void ComputeBounds( unify::BBox< float > & bbox ) const;

		bool IsTrans() const;

	protected:
		me::IRenderer * m_renderer;
		std::vector< BufferSet::shared_ptr > m_buffers;
		unify::FrameSet m_frameSet;
		frameanimation::AnimationSet m_animationSet;
	};
}