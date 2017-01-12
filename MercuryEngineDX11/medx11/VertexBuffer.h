// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <medx11/Renderer.h>
#include <me/IVertexBuffer.h>
#include <me/BufferUsage.h>
#include <me/VertexDeclaration.h>
#include <unify/BBox.h>
#include <atlbase.h>

namespace medx11
{
	class VertexBuffer : public me::IVertexBuffer
	{
	public:
		VertexBuffer( const me::IRenderer * renderer );
		VertexBuffer( const me::IRenderer * renderer, me::VertexBufferParameters parameters );
		~VertexBuffer();

		void Create( me::VertexBufferParameters parameters ) override;
		void Destroy();

		void Lock( unify::DataLock & lock );
		void LockReadOnly( unify::DataLock & lock ) const;
		void Unlock();
		void Unlock() const;
		
		me::VertexDeclaration::ptr GetVertexDeclaration() const override;
		
		bool Valid() const;
		void Use() const override;

		unify::BBox< float > & GetBBox() override;
		const unify::BBox< float > & GetBBox() const override;

		bool Locked() const;
		me::BufferUsage::TYPE GetUsage() const;
		unsigned int GetStride() const;
		unsigned int GetLength() const;
		size_t GetSizeInBytes() const override;

	protected:
		const Renderer * m_renderer;
		std::vector< ID3D11Buffer * > m_buffers;

		me::VertexDeclaration::ptr m_vertexDeclaration;

		unify::BBox< float > m_bbox;

		std::vector< bool > m_locked;
		me::BufferUsage::TYPE m_usage;
		std::vector< size_t > m_strides; // Size of each item in the buffer.
		std::vector< size_t > m_lengths; // Number of items we can store in the buffer.
	};
}