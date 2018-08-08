// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/render/PrimitiveList.h>
#include <unify/TexCoords.h>
#include <unify/String.h>

using namespace me;
using namespace render;

PrimitiveList::PrimitiveList( IRenderer * renderer )
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

void PrimitiveList::Render( const render::Params & params, MatrixFeed & matrixFeed ) const
{
	for( const auto & set : m_buffers )
	{	
		set->Render( params, matrixFeed );
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

bool PrimitiveList::IsTrans() const
{
	for( const auto & bs : m_buffers )
	{
		for( const auto & method : bs->GetRenderMethodBuffer() )
		{
			if( method.IsTrans() )
			{
				return true;
			}
		}
	}
	return false;
}
