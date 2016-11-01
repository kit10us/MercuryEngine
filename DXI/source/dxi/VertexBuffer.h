// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IRenderer.h>
#include <dxi/DataBuffer.h>
#include <dxi/VertexDeclaration.h>
#include <unify/BBox.h>

namespace dxi
{
	class VertexBuffer
	{
	public:
		typedef std::shared_ptr< VertexBuffer > ptr;

		VertexBuffer( core::IRenderer * renderer );
		VertexBuffer( core::IRenderer * renderer, unsigned int numVertices, VertexDeclaration::ptr vertexDeclaration, size_t slot, const void * source, BufferUsage::TYPE usage = BufferUsage::Default );
		~VertexBuffer();

		void Create( unsigned int numVertices, VertexDeclaration::ptr vertexDeclaration, size_t slot,  const void * source, BufferUsage::TYPE usage = BufferUsage::Default );
		void Destroy();

		void Lock( unify::DataLock & lock );
		void LockReadOnly( unify::DataLock & lock ) const;
		void Unlock();
		void Unlock() const;
		
		VertexDeclaration::ptr GetVertexDeclaration() const;
		size_t GetSlot() const;
		
		bool Valid() const;
		void Use() const;

		unify::BBox< float > & GetBBox();
		const unify::BBox< float > & GetBBox() const;

		bool Locked() const;
		BufferUsage::TYPE GetUsage() const;
		unsigned int GetStride() const;
		unsigned int GetLength() const;
		unsigned int GetSize() const;

	protected:
		class Pimpl;
		std::shared_ptr< Pimpl > m_pimpl;

		VertexDeclaration::ptr m_vertexDeclaration;
		size_t m_slot;

		unify::BBox< float > m_bbox;

		bool m_locked;
		BufferUsage::TYPE m_usage;
		unsigned int m_stride; // Size of each item in the buffer.
		unsigned int m_length; // Number of items we can store in the buffer.
	};
}