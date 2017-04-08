// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/render/RenderParams.h>
#include <me/render/MatrixFeed.h>
#include <me/render/IVertexBuffer.h>
#include <me/render/IIndexBuffer.h>
#include <me/render/RenderMethod.h>

namespace me
{
	namespace render
	{
		///<summary>
		/// A set of buffers, the best way to allow multiple vertex types is to bind the three buffers togther.
		///</summary>
		class BufferSet
		{
		public:
			typedef std::shared_ptr< BufferSet > shared_ptr;

			BufferSet( me::render::IRenderer * renderer );
			~BufferSet();

			BufferSet( const BufferSet & ) = delete;

			IVertexBuffer::ptr BufferSet::AddVertexBuffer( VertexBufferParameters parameters );
			IVertexBuffer::ptr BufferSet::GetVertexBuffer();
			const IVertexBuffer::ptr BufferSet::GetVertexBuffer() const;

			IIndexBuffer::ptr BufferSet::AddIndexBuffer( IndexBufferParameters parameters );
			IIndexBuffer::ptr BufferSet::GetIndexBuffer();
			const IIndexBuffer::ptr BufferSet::GetIndexBuffer() const;

			void AddMethod( RenderMethod & method );
			std::vector< RenderMethod > & GetRenderMethodBuffer();
			const std::vector< RenderMethod > & GetRenderMethodBuffer() const;
			void ClearMethods();

			void Destroy();

			void SetEnabled( bool enabled );
			bool GetEnabled() const;

			void Render( render::Params params, render::MatrixFeed & matrixFeed ) const;

		private:
			IRenderer * m_renderer;
			bool m_enabled;
			IVertexBuffer::ptr m_VB;
			IIndexBuffer::ptr m_IB;
			std::vector< RenderMethod > m_RB;
		};
	}
}