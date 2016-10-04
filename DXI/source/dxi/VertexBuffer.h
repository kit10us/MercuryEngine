// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/DataBuffer.h>
#include <dxi/VertexDeclaration.h>

namespace dxi
{
	// Quick encapsulation of vertex and index buffers...
	class VertexBuffer : public DataBuffer
	{
	public:
		typedef std::shared_ptr< VertexBuffer > ptr;

		// TODO: Replace create to use this.  Move to DataBuffer???
		struct BufferDesc
		{		  
			BufferDesc()
				: byteWidth( 0 )
				, usage( BufferUsage::Default )
				, stride( 0 )
			{}

			size_t byteWidth;
			BufferUsage::TYPE usage;
			size_t stride;
		};

		VertexBuffer();
		VertexBuffer( unsigned int numVertices, VertexDeclaration::ptr vertexDeclaration, const void * source, BufferUsage::TYPE usage = BufferUsage::Default );
		~VertexBuffer();

		void Create( unsigned int numVertices, VertexDeclaration::ptr vertexDeclaration, const void * source, BufferUsage::TYPE usage = BufferUsage::Default );

		void Resize( unsigned int numVertices );

		/// <summary>
		/// Append into this vertex buffer the entire contents of another vertex buffer, returning
		/// the offset (where the new vertices are copied to, i.e., the index of the first vertex
		/// in the new vertex buffer).
		/// </summary>
		size_t Append( const VertexBuffer & vb );

		void Destroy();

		void Lock( unify::DataLock & lock );
		void LockReadOnly( unify::DataLock & lock ) const;
		void Unlock();
		void Unlock() const;
		void Upload( const void * pVerticesIn, unsigned int uStartVert, unsigned int uNumVerts );
		VertexDeclaration::ptr GetVertexDeclaration() const;
		bool Valid() const;
		void Use() const;
		void Disuse() const;
		
	protected:	   
		class Pimpl;
		std::shared_ptr< Pimpl > m_pimpl;
		VertexDeclaration::ptr m_vertexDeclaration;
	};
}