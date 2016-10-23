// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IRenderer.h>
#include <dxi/DataBuffer.h>
#include <dxi/IndexLock.h>
#include <unify/unify.h>
#include <unify/Flags.h>
#include <memory>

namespace dxi
{
	// Typedefs...
	typedef unsigned long			Index32;

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

		IndexBuffer( core::IRenderer * renderer );
		~IndexBuffer();

		void Create( unsigned int numIndices, Index32 * source, BufferUsage::TYPE usage = BufferUsage::Default, unify::Flags flags = 0 );
		void Resize( unsigned int numIndices );

		/// <summary>
		/// Append to our existing indices, the indices from ib, adding a vertex offset to each new index.
		/// Returns the index offset for the first index from 'from'.
		/// </summary>
		size_t Append( const IndexBuffer & from, size_t vertexOffset = 0 );
		void Destroy();

		void Lock( IndexLock & lock );
		void LockReadOnly( IndexLock & lock ) const;
		void Unlock();
		void UnlockReadOnly() const;
		unsigned int GetCreateFlags() const;
		bool Valid() const;
		void Use() const;

	protected:
		class Pimpl;
		std::shared_ptr< Pimpl > m_pimpl;
	};
}