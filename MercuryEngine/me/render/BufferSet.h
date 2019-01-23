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

			/// <summary>
			/// Construct a BufferSet from an Effect. The constant buffers are automatically created from the effect.
			/// </summary>
			BufferSet( me::render::IRenderer * renderer, Effect::ptr effect );

			~BufferSet();

			BufferSet( const BufferSet & ) = delete;

			/// <summary>
			/// Set effect. Automatically creates constant buffers.
			/// </summary>
			void SetEffect( Effect::ptr effect );
			Effect::ptr GetEffect() const;

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

			/// <summary>
			/// Get vertex constant buffer.
			/// </summary>
			IConstantBuffer* GetVertexCB();

			/// <summary>
			/// Get vertex constant buffer.
			/// </summary>
			const IConstantBuffer* GetVertexCB() const;

			/// <summary>
			/// Get pixel constant buffer.
			/// </summary>
			IConstantBuffer* GetPixelCB();

			/// <summary>
			/// Get pixel constant buffer.
			/// </summary>
			const IConstantBuffer* GetPixelCB() const;

			void Destroy();

			void SetEnabled( bool enabled );
			bool GetEnabled() const;

			void Render( const render::Params & params, render::MatrixFeed & matrixFeed ) const;

		private:
			IRenderer * m_renderer;
			bool m_enabled;
			Effect::ptr m_effect;
			IConstantBuffer::ptr m_vertexCB;
			IConstantBuffer::ptr m_pixelCB;
			IVertexBuffer::ptr m_VB;
			IIndexBuffer::ptr m_IB;
			std::vector< RenderMethod > m_RB;
		};
	}
}