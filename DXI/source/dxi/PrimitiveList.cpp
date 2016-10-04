// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/PrimitiveList.h>
#include <dxi/core/Game.h>
#include <unify/TexCoords.h>
#include <unify/String.h>

using namespace dxi;

PrimitiveList::PrimitiveList()
{
}

PrimitiveList::~PrimitiveList()
{
	Destroy();
}

void PrimitiveList::Append( const PrimitiveList & from )
{
	for( std::vector< BufferSet::shared_ptr >::const_iterator itrFrom = from.m_buffers.begin(), endFrom = from.m_buffers.end(); itrFrom != endFrom; ++itrFrom )
	{
		const BufferSet & fromSet = *itrFrom->get();
		bool bufferFound = false;
		for( std::vector< BufferSet::shared_ptr >::iterator itrTo = m_buffers.begin(), endTo = m_buffers.end(); itrTo != endTo && bufferFound == false; ++itrTo )
		{
			BufferSet & toSet = *itrTo->get();
			if ( toSet.GetVertexBuffer().GetVertexDeclaration() == fromSet.GetVertexBuffer().GetVertexDeclaration() )
			{
				size_t vbOffset = toSet.GetVertexBuffer().Append( fromSet.GetVertexBuffer() );
				size_t ibOffset = toSet.GetIndexBuffer().Append( fromSet.GetIndexBuffer(), vbOffset );
				size_t rbOffset = toSet.GetRenderMethodBuffer().Append( fromSet.GetRenderMethodBuffer(), vbOffset, ibOffset );
				bufferFound = true;
			}
		}

		if ( bufferFound == false )
		{
			BufferSet & toSet = AddBufferSet();		
			toSet.GetVertexBuffer().Append( fromSet.GetVertexBuffer() );
			toSet.GetIndexBuffer().Append( fromSet.GetIndexBuffer() );
			toSet.GetRenderMethodBuffer().Append( fromSet.GetRenderMethodBuffer(), 0, 0 );
		}
	}
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
	BufferSet::shared_ptr newSet( new BufferSet );
	m_buffers.push_back( newSet );
	return *newSet.get();
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

const unify::BBox< float > & PrimitiveList::ComputeBounds( unify::BBox< float > & boundingBox ) const
{
	boundingBox.Initialize();

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
	return boundingBox;
}
