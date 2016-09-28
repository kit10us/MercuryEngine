// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/DataBuffer.h>
#include <dxi/IndexLock.h>
#include <unify/unify.h>
#include <dxi/win/DirectX.h>
#include <unify/Flags.h>
#include <memory>
#include <atlbase.h>

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

		IndexBuffer();
		~IndexBuffer();

		void Create( unsigned int numIndices, BufferUsage::TYPE usage = BufferUsage::Default, Index32 * source = nullptr, unify::Flags flags = 0 );
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

	protected:
		unsigned int m_createFlags;

#if defined( DIRECTX9 )
		CComPtr< IDirect3DIndexBuffer9 > m_IB;
#elif defined( DIRECTX11 )
		CComPtr< ID3D11Buffer > m_IB;
#endif
	};
}