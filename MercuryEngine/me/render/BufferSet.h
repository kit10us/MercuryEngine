// Copyright (c) 2002 - 2018, Evil Quail LLC
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
			typedef std::shared_ptr< BufferSet > ptr;

			BufferSet( me::render::IRenderer * renderer );
			~BufferSet();

			BufferSet( const BufferSet & ) = delete;

			IVertexBuffer::ptr AddVertexBuffer( VertexBufferParameters parameters );
			IVertexBuffer::ptr GetVertexBuffer();
			const IVertexBuffer::ptr GetVertexBuffer() const;

			IIndexBuffer::ptr AddIndexBuffer( IndexBufferParameters parameters );
			IIndexBuffer::ptr GetIndexBuffer();
			const IIndexBuffer::ptr GetIndexBuffer() const;

			void AddMethod( RenderMethod & method );
			std::vector< RenderMethod > & GetRenderMethodBuffer();
			const std::vector< RenderMethod > & GetRenderMethodBuffer() const;
			void ClearMethods();

			void Destroy();

			void SetEnabled( bool enabled );
			bool GetEnabled() const;

			void Render( const render::Params & params, render::MatrixFeed & matrixFeed ) const;

		private:
			IRenderer * m_renderer;
			bool m_enabled;
			IVertexBuffer::ptr m_VB;
			IIndexBuffer::ptr m_IB;
			std::vector< RenderMethod > m_RB;
		};
	}
}