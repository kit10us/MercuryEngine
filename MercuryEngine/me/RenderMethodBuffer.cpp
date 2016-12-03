// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/RenderMethodBuffer.h>
#include <me/IRenderer.h>
#include <me/exception/FailedToCreate.h>
#include <me/exception/FailedToLock.h>
#include <me/exception/Exception.h>
#include <assert.h>

using namespace me;

RenderMethodBuffer::RenderMethodBuffer()
{
}

void RenderMethodBuffer::Clear()
{
	m_methodList.clear();
}

void RenderMethodBuffer::AddMethod( RenderMethod & method )
{
	m_methodList.push_back( method );
}

size_t RenderMethodBuffer::Append( const RenderMethodBuffer & from, size_t vertexOffset, size_t indexOffset )
{
	for( size_t m = 0; m < from.m_methodList.size(); ++m )
	{
		const RenderMethod & methodSource = from.m_methodList[ m ];
		RenderMethod method( methodSource );
		method.startVertex += vertexOffset;
		method.baseVertexIndex = 0;
		method.minIndex += indexOffset;
		//method.vertexCount;
		method.startIndex += indexOffset;
		//method.primitiveCount;
		m_methodList.push_back( method );
	}
	return from.GetSize();
}

RenderMethod & RenderMethodBuffer::GetMethod( unsigned int method )
{
	return m_methodList[ method ];
}

const RenderMethod & RenderMethodBuffer::GetMethod( unsigned int method ) const
{
	return m_methodList[ method ];
}

size_t RenderMethodBuffer::GetSize() const
{
	return m_methodList.size();
}

void RenderMethodBuffer::Render( IRenderer * renderer, const RenderInfo & renderInfo, const unify::Matrix & world ) const
{
	// Iterate through methods to render.
	for( unsigned int i = 0; i != m_methodList.size(); ++i )
	{
		const RenderMethod & method = m_methodList[ i ];
		renderer->Render( method, renderInfo, world );
	}
}

void RenderMethodBuffer::Render( IRenderer * renderer, const RenderInfo & renderInfo, std::vector< const unify::FrameLite * > & instances ) const
{
	// Iterate through methods to render.
	for( unsigned int i = 0; i != m_methodList.size(); ++i )
	{
		const RenderMethod & method = m_methodList[ i ];
		renderer->RenderInstanced( method, renderInfo, instances );
	}
}