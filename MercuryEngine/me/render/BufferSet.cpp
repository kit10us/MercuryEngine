// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/render/BufferSet.h>
#include <me/render/IRenderer.h>

using namespace me;
using namespace render;

BufferSet::BufferSet(me::render::IRenderer * renderer )
	: m_renderer( renderer )
	, m_enabled( true )
{
}

BufferSet::~BufferSet()
{
}

IVertexBuffer::ptr BufferSet::AddVertexBuffer( VertexBufferParameters parameters )
{
	m_VB = m_renderer->ProduceVB( parameters );
	return m_VB;
}

IVertexBuffer::ptr BufferSet::GetVertexBuffer()
{
	return m_VB;
}

const IVertexBuffer::ptr BufferSet::GetVertexBuffer() const
{
	return m_VB;
}

IIndexBuffer::ptr BufferSet::AddIndexBuffer( IndexBufferParameters parameters )
{
	m_IB = m_renderer->ProduceIB( parameters );
	return m_IB;
}

IIndexBuffer::ptr BufferSet::GetIndexBuffer()
{
	return m_IB;
}

const IIndexBuffer::ptr BufferSet::GetIndexBuffer() const
{
	return m_IB;
}

void BufferSet::AddMethod( RenderMethod & method )
{
	m_RB.push_back( method );
}

std::vector< RenderMethod > & BufferSet::GetRenderMethodBuffer()
{
	return m_RB;
}

const std::vector< RenderMethod > & BufferSet::GetRenderMethodBuffer() const
{
	return m_RB;
}

void BufferSet::ClearMethods()
{
	m_RB.clear();
}

void BufferSet::Destroy()
{
	m_VB.reset();
	m_IB.reset();
	m_RB.clear();
}

void BufferSet::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

bool BufferSet::GetEnabled() const
{
	return m_enabled;
}

void BufferSet::Render( const render::Params & params, render::MatrixFeed & matrixFeed ) const
{
	if ( !m_enabled ) return;

	if( m_VB )
	{
		m_VB->Use();
	}

	if ( m_IB )
	{
		m_IB->Use();
	}

	// Iterate through methods to render.
	for( auto && method : m_RB )
	{
		params.renderer->Render( method, params.renderInfo, matrixFeed );
		matrixFeed.Restart();
	}
}

