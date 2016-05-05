// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/DataBuffer.h>
#include <dxi/IndexLock.h>
#include <unify/unify.h>
#include <dxi/win/DirectX.h>
#include <unify/Flags.h>
#include <memory>

namespace dxi
{
	// Typedefs...
	typedef unsigned short			Index16;
	typedef unsigned long			Index32;

	struct IndexFormat
	{
		enum TYPE
		{
			Index16,
			Index32
		};
	};

	class IndexBuffer : public DataBuffer
	{
	public:
		typedef  std::shared_ptr< IndexBuffer > shared_ptr;

		struct CreateFlags
		{
			enum TYPE
			{
				DoNotClip = FLAG01,
				Dynamic = FLAG02,
				NPatches = FLAG03,
				Points = FLAG04,
				RTPatches = FLAG05,
				SoftwareProcessing = FLAG06,
				WriteOnly = FLAG07,
			};
		};

		IndexBuffer();
		~IndexBuffer();

		void Create( unsigned int numIndices, IndexFormat::TYPE format = IndexFormat::Index32, BufferUsage::TYPE usage = BufferUsage::Default, unify::Flags flags = 0 );
		void Resize( unsigned int numIndices );

		/// <summary>
		/// Append to our existing indices, the indices from ib, adding a vertex offset to each new index.
		/// Returns the index offset for the first index from 'from'.
		/// </summary>
		size_t Append( const IndexBuffer & from, size_t vertexOffset = 0 );
		void Release();

		void Lock( IndexLock & lock );
		void LockReadOnly( IndexLock & lock ) const;
		void Unlock();
		void UnlockReadOnly() const;
		unsigned int GetCreateFlags() const;
		bool Valid() const;
		void Use() const;
		void Disuse() const;
		IndexFormat::TYPE GetFormat() const;

	protected:
		IndexFormat::TYPE m_format;
		unsigned int m_createFlags;
		IDirect3DIndexBuffer9 *	m_IB;
	};
}