// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <medx11/Renderer.h>
#include <me/IIndexBuffer.h>
#include <me/BufferUsage.h>
#include <me/IndexLock.h>
#include <me/BufferUsage.h>
#include <unify/unify.h>
#include <unify/Flags.h>
#include <atlbase.h>
#include <memory>

namespace medx11
{
	class IndexBuffer : public me::IIndexBuffer
	{
	public:
		IndexBuffer( me::IRenderer * renderer );
		IndexBuffer( me::IRenderer * renderer, me::IndexBufferParameters parameters );
		~IndexBuffer();

		void Create( me::IndexBufferParameters parameters );
		void Resize( size_t bufferIndex, unsigned int numIndices );

		/// <summary>
		/// Append to our existing indices, the indices from ib, adding a vertex offset to each new index.
		/// Returns the index offset for the first index from 'from'.
		/// </summary>
		size_t Append( size_t bufferIndex, const IndexBuffer & from, size_t vertexOffset = 0 );
		void Destroy();

		void Lock( size_t bufferIndex, unify::DataLock & lock ) override;
		void LockReadOnly( size_t bufferIndex, unify::DataLock & lock ) const override;
		void Unlock( size_t bufferIndex, unify::DataLock & lock ) override;
		void UnlockReadOnly( size_t bufferIndex, unify::DataLock & lock ) const override;

		bool Valid() const;
		
		void Use() const override;

		bool Locked( size_t bufferIndex ) const override;
		me::BufferUsage::TYPE GetUsage( size_t bufferIndex ) const override;
		size_t GetStride( size_t bufferIndex ) const override;
		size_t GetLength( size_t bufferIndex ) const override;
		size_t GetSizeInBytes( size_t bufferIndex ) const override;

	protected:
		const Renderer * m_renderer;
		unsigned int m_createFlags;
		CComPtr< ID3D11Buffer > m_buffer;
		mutable bool m_locked;
		me::BufferUsage::TYPE m_usage;
		unsigned int m_stride; // Size of each item in the buffer.
		unsigned int m_length; // Number of items we can store in the buffer.
	};
}