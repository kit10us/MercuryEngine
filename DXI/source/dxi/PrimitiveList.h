// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/BufferSet.h>
#include <dxi/RenderInfo.h>
#include <dxi/frameanimation/FrameAnimationSet.h>
#include <unify/BBox.h>
#include <unify/BSphere.h>
#include <unify/Frame.h>

namespace dxi
{
	///<summary>
	/// Holds data on how to render a set of primitives.
	///</summary>
	class PrimitiveList
	{
	public:
		PrimitiveList();
		~PrimitiveList();

		PrimitiveList( const PrimitiveList & ) = delete;

		/// <summary>
		/// Append an existing PrimtiveList onto this one.
		/// </summary>
		void Append( const PrimitiveList & from );

		void Destroy();

		void Render( const RenderInfo & renderInfo ) const;
		
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

	protected:
		std::vector< BufferSet::shared_ptr > m_buffers;
		unify::FrameSet m_frameSet;
		frameanimation::AnimationSet m_animationSet;
	};
}