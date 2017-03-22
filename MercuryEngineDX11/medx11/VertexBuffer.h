// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <medx11/Renderer.h>
#include <me/IVertexBuffer.h>
#include <unify/BBox.h>
#include <atlbase.h>

namespace medx11
{
	class VertexBuffer : public me::IVertexBuffer
	{
	public:
		VertexBuffer( me::IRenderer * renderer );
		VertexBuffer( me::IRenderer * renderer, me::VertexBufferParameters parameters );
		~VertexBuffer();

		void Create( me::VertexBufferParameters parameters ) override;
		void Destroy();

		void Lock( size_t bufferIndex, unify::DataLock & lock ) override;
		void LockReadOnly( size_t bufferIndex, unify::DataLock & lock ) const override;
		void Unlock( size_t bufferIndex, unify::DataLock & lock ) override;
		void UnlockReadOnly( size_t bufferIndex, unify::DataLock & lock ) const override;
		
		me::VertexDeclaration::ptr GetVertexDeclaration() const override;
		
		bool Valid() const;
		void Use() const override;

		unify::BBox< float > & GetBBox() override;
		const unify::BBox< float > & GetBBox() const override;

		bool Locked( size_t bufferIndex ) const override;
		me::BufferUsage::TYPE GetUsage( size_t bufferIndex ) const override;
		size_t GetStride( size_t bufferIndex ) const override;
		size_t GetLength( size_t bufferIndex ) const override;
		size_t GetSizeInBytes( size_t bufferIndex ) const override;

	protected:
		const Renderer * m_renderer;
		std::vector< ID3D11Buffer * > m_buffers;

		me::VertexDeclaration::ptr m_vertexDeclaration;

		unify::BBox< float > m_bbox;

		mutable std::vector< bool > m_locked;
		std::vector< me::BufferUsage::TYPE > m_usage;
		std::vector< size_t > m_strides; // Size of each item in the buffer.
		std::vector< size_t > m_lengths; // Number of items we can store in the buffer.
	};
}