// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/PrimitiveList.h>
#include <unify/TexCoords.h>
#include <unify/String.h>

using namespace me;

PrimitiveList::PrimitiveList( const me::IRenderer * renderer )
	: m_renderer( renderer )
{
}

PrimitiveList::~PrimitiveList()
{
	Destroy();
}

// Destroy all contents...
void PrimitiveList::Destroy()
{
	m_buffers.clear();
}

void PrimitiveList::Render( IRenderer * renderer, const me::RenderInfo & renderInfo, const unify::FrameLite ** instances, const size_t instances_size ) const
{
	for( const auto & set : m_buffers )
	{
		if( ! set->GetEnabled() ) return;

		if( set->GetVertexBuffer() )
		{
			set->GetVertexBuffer()->Use();
		}

		if ( set->GetIndexBuffer() )
		{
			set->GetIndexBuffer()->Use();
		}

		set->GetRenderMethodBuffer().Render( renderer, renderInfo, instances, instances_size );
	}
}

void PrimitiveList::Render( IRenderer * renderer, const me::RenderInfo & renderInfo, const InstancesSet * instancesList, const size_t instancesList_size ) const
{
	for( const auto & set : m_buffers )
	{
		if( ! set->GetEnabled() ) return;

		if( set->GetVertexBuffer() )
		{
			set->GetVertexBuffer()->Use();
		}

		if ( set->GetIndexBuffer() )
		{
			set->GetIndexBuffer()->Use();
		}

		set->GetRenderMethodBuffer().Render( renderer, renderInfo, instancesList, instancesList_size );
	}
}

void PrimitiveList::Render( IRenderer * renderer, const RenderInfo & renderInfo, const IMatrixSource * sources, const size_t sources_size, bool contiguous ) const
{
	for( const auto & set : m_buffers )
	{
		if( ! set->GetEnabled() ) return;

		if( set->GetVertexBuffer() )
		{
			set->GetVertexBuffer()->Use();
		}

		if ( set->GetIndexBuffer() )
		{
			set->GetIndexBuffer()->Use();
		}

		set->GetRenderMethodBuffer().Render( renderer, renderInfo, sources, sources_size, contiguous );
	}
}

BufferSet & PrimitiveList::AddBufferSet()
{
	BufferSet::shared_ptr newSet( new BufferSet( m_renderer ) );
	m_buffers.push_back( newSet );
	return *newSet.get();
}

void PrimitiveList::RemoveBufferSet( size_t index )
{
	m_buffers.erase( m_buffers.begin() + index );
}

void PrimitiveList::RemoveAllBufferSets()
{
	m_buffers.clear();
}

size_t PrimitiveList::GetBufferSetCount() const
{
	return m_buffers.size();
}

BufferSet & PrimitiveList::GetBufferSet( size_t index )
{
	return *m_buffers[ index ].get();
}

const BufferSet & PrimitiveList::GetBufferSet( size_t index ) const
{
	return *m_buffers[ index ].get();
}

const unify::FrameSet & PrimitiveList::GetFrameSet() const
{
	return m_frameSet;
}

unify::FrameSet & PrimitiveList::GetFrameSet()
{
	return m_frameSet;
}

frameanimation::AnimationSet & PrimitiveList::GetAnimationSet()
{
	return m_animationSet;
}

const frameanimation::AnimationSet & PrimitiveList::GetAnimationSet() const
{
	return m_animationSet;
}

void PrimitiveList::ComputeBounds( unify::BBox< float > & bbox ) const
{
	bbox.Clear();

	for ( const auto & bs : m_buffers )
	{
		const IVertexBuffer::ptr vb = bs->GetVertexBuffer();
		bbox += vb->GetBBox();
	}
}
