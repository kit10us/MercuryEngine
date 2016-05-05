// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/DataBuffer.h>
#include <dxi/win/DXDevice.h>
#include <dxi/VertexDeclaration.h>
#include <unify/Flags.h>
#include <unify/Matrix.h>
#include <unify/DataLock.h>
#include <memory>

namespace dxi
{
	// Quick encapsulation of vertex and index buffers...
	//										---EXCLUSIVE---
	#define TRANSFORM_COORD		FLAG01	//	*
	#define TRANSFORM_NORMAL	FLAG02	//	*
	#define TRANSFORM_2D		FLAG03	//		*
	#define TRANSFORM_4D		FLAG04	//		*

	class VertexBuffer : public DataBuffer
	{
	public:
		typedef std::shared_ptr< VertexBuffer > shared_ptr;

		struct CreateFlags
		{
			enum TYPE
			{
				DoNotClip = FLAG01,
				Dynamic = FLAG02,
				NPatches = FLAG03,
				Points = FLAG04,
				RTPatches = FLAG05,
			};
		};

		VertexBuffer();
		VertexBuffer( unsigned int numVertices, VertexDeclaration vertexDeclaration, BufferUsage::TYPE usage = BufferUsage::Default, unify::Flags flags = FLAGNULL );
		~VertexBuffer();

		void Create( unsigned int numVertices, VertexDeclaration vertexDeclaration, BufferUsage::TYPE usage = BufferUsage::Default, unify::Flags flags = FLAGNULL );

		void Resize( unsigned int numVertices );

		/// <summary>
		/// Append into this vertex buffer the entire contents of another vertex buffer, returning
		/// the offset (where the new vertices are copied to, i.e., the index of the first vertex
		/// in the new vertex buffer).
		/// </summary>
		size_t Append( const VertexBuffer & vb );

		void Release();

		void Lock( unify::DataLock & lock );
		void LockReadOnly( unify::DataLock & lock ) const;
		void Unlock();
		void Unlock() const;
		void Upload( const void * pVerticesIn, unsigned int uStartVert, unsigned int uNumVerts );
		VertexDeclaration GetVertexDeclaration() const;
		unsigned int GetCreateFlags() const;
		bool Valid() const;
		void Use( unsigned int streamNumber = 0, unsigned int offsetInBytes = 0, unsigned int stride = 0 ) const;
		void Disuse() const;
		
	protected:
		IDirect3DVertexBuffer9 * m_VB;
		unify::Flags m_createFlags;
		VertexDeclaration m_vertexDeclaration;
	};
}