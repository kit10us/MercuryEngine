// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/VertexBuffer.h>
#include <dxi/IndexBuffer.h>
#include <dxi/RenderMethodBuffer.h>

namespace dxi
{
	///<summary>
	/// A set of buffers, the best way to allow multiple vertex types is to bind the three buffers togther.
	///</summary>
	class BufferSet
	{
	public:
		typedef std::shared_ptr< BufferSet > shared_ptr;

		BufferSet();
		~BufferSet();

		VertexBuffer & BufferSet::GetVertexBuffer();
		const VertexBuffer & BufferSet::GetVertexBuffer() const;

		IndexBuffer & BufferSet::GetIndexBuffer();
		const IndexBuffer & BufferSet::GetIndexBuffer() const;

		RenderMethodBuffer & BufferSet::GetRenderMethodBuffer();
		const RenderMethodBuffer & BufferSet::GetRenderMethodBuffer() const;

	private:
		VertexBuffer m_VB;
		IndexBuffer m_IB;
		RenderMethodBuffer m_RB;
	};
}