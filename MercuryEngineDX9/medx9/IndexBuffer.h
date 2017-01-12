// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <medx9/IndexBuffer.h>
#include <medx9/Renderer.h>
#include <unify/unify.h>
#include <unify/Flags.h>
#include <memory>

namespace medx9
{
	class IndexBuffer : public me::IIndexBuffer
	{
	public:
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

		IndexBuffer( const me::IRenderer * renderer );
		IndexBuffer( const me::IRenderer * renderer, me::IndexBufferParameters parameters );
		~IndexBuffer();

		void Create( me::IndexBufferParameters parameters );

		void Destroy();

		void Lock( unify::DataLock & lock ) override;
		void LockReadOnly( unify::DataLock & lock ) const override;
		void Unlock( unify::DataLock & lock ) override;
		void UnlockReadOnly( unify::DataLock & lock ) const override;

		bool Valid() const;
		
		void Use() const override;

		bool Locked() const;
		me::BufferUsage::TYPE GetUsage() const;
		unsigned int GetStride() const;
		unsigned int GetLength() const;
		size_t GetSizeInBytes() const override;

	protected:
		const Renderer * m_renderer;
		unsigned int m_createFlags;
		IDirect3DIndexBuffer9 * m_buffer;
		bool m_locked;
		me::BufferUsage::TYPE m_usage;
		unsigned int m_stride; // Size of each item in the buffer.
		unsigned int m_length; // Number of items we can store in the buffer.
	};
}