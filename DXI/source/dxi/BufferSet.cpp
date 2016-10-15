// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/BufferSet.h>

using namespace dxi;

BufferSet::BufferSet()
	: m_enabled( true )
{
}

BufferSet::~BufferSet()
{
}

VertexBuffer & BufferSet::GetVertexBuffer()
{
	return m_VB;
}

const VertexBuffer & BufferSet::GetVertexBuffer() const
{
	return m_VB;
}

IndexBuffer & BufferSet::GetIndexBuffer()
{
	return m_IB;
}

const IndexBuffer & BufferSet::GetIndexBuffer() const
{
	return m_IB;
}

RenderMethodBuffer & BufferSet::GetRenderMethodBuffer()
{
	return m_RB;
}

const RenderMethodBuffer & BufferSet::GetRenderMethodBuffer() const
{
	return m_RB;
}

void BufferSet::Destroy()
{
	m_VB.Destroy();
	m_IB.Destroy();
	m_RB.Clear();
}

void BufferSet::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

bool BufferSet::GetEnabled() const
{
	return m_enabled;
}
