// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/PrimitiveList.h>
#include <unify/TexCoords.h>
#include <unify/String.h>

using namespace dxi;

PrimitiveList::PrimitiveList( core::IRenderer * renderer )
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

void PrimitiveList::Render( const RenderInfo & renderInfo ) const
{
	for( const auto & set : m_buffers )
	{
		if( ! set->GetEnabled() ) continue;

		set->GetVertexBuffer().Use();
		set->GetIndexBuffer().Use();
		set->GetRenderMethodBuffer().Render( renderInfo );
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

	/*
	VertexElement positionE = CommonVertexElement::Position();

	// Loop through vertices for largest radius
	for( const auto & bs : m_buffers )
	{
		unify::DataLock lock;
		const VertexBuffer & vb = bs->GetVertexBuffer();

		vb.LockReadOnly( lock );
		VertexDeclaration::ptr vd = vb.GetVertexDeclaration();

		for( size_t index = 0; index < vb.GetLength(); index++ )
		{
			unify::V3< float > pos;
			vd->ReadVertex( lock, index, positionE, pos );
			boundingBox.AddPoint( pos );
		}
		vb.Unlock();
	}
	*/

	for ( const auto & bs : m_buffers )
	{
		const VertexBuffer & vb = bs->GetVertexBuffer();
		bbox += vb.GetBBox();
	}
}
