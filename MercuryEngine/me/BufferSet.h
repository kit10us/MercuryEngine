// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/RenderParams.h>
#include <me/MatrixFeed.h>
#include <me/IVertexBuffer.h>
#include <me/IIndexBuffer.h>
#include <me/RenderMethod.h>

namespace me
{
	///<summary>
	/// A set of buffers, the best way to allow multiple vertex types is to bind the three buffers togther.
	///</summary>
	class BufferSet
	{
	public:
		typedef std::shared_ptr< BufferSet > shared_ptr;

		BufferSet( const me::IRenderer * renderer );
		~BufferSet();

		BufferSet( const BufferSet & ) = delete;

		me::IVertexBuffer::ptr BufferSet::AddVertexBuffer( VertexBufferParameters parameters );
		me::IVertexBuffer::ptr BufferSet::GetVertexBuffer();
		const me::IVertexBuffer::ptr BufferSet::GetVertexBuffer() const;

		me::IIndexBuffer::ptr BufferSet::AddIndexBuffer( IndexBufferParameters parameters );
		me::IIndexBuffer::ptr BufferSet::GetIndexBuffer();
		const me::IIndexBuffer::ptr BufferSet::GetIndexBuffer() const;

		void AddMethod( RenderMethod & method );
		std::vector< RenderMethod > & BufferSet::GetRenderMethodBuffer();
		const std::vector< RenderMethod > & BufferSet::GetRenderMethodBuffer() const;
		void ClearMethods();

		void Destroy();

		void SetEnabled( bool enabled );
		bool GetEnabled() const;

		void Render( RenderParams params, MatrixFeed & matrixFeed ) const;

	private:
		const IRenderer * m_renderer;
		bool m_enabled;
		me::IVertexBuffer::ptr m_VB;
		me::IIndexBuffer::ptr m_IB;
		std::vector< RenderMethod > m_RB;
	};
}