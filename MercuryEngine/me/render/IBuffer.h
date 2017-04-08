// Copyright (c) 2003 - 2017, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/render/BufferUsage.h>
#include <me/render/IndexLock.h>
#include <unify/unify.h>
#include <unify/Flags.h>
#include <memory>

namespace me
{
	namespace render
	{
		struct BufferParameters
		{
			BufferParameters()
				: sizeInBytes{ 0 }
				, usage{ me::render::BufferUsage::Default }
				, source{ 0 }
				, flags{ 0 }
			{
			}

			BufferParameters( size_t _sizeInBytes, me::render::BufferUsage::TYPE _usage = me::render::BufferUsage::Default, void * source = 0, unify::Flags _flags = 0 )
				: sizeInBytes{ _sizeInBytes }
				, usage{ _usage }
				, source{ source }
				, flags{ _flags }
			{
			}

			size_t sizeInBytes;
			me::render::BufferUsage::TYPE usage;
			unify::Flags flags;
			void * source;
			unsigned int bindFlags;
		};

		class IBuffer
		{
		public:
			typedef std::shared_ptr< IBuffer > ptr;
			virtual ~IBuffer() {}
			//virtual void Create( BufferParameters parameters ) = 0;
			virtual void Destroy() = 0;
			virtual void Use() const = 0;

			virtual void Lock( size_t bufferIndex, unify::DataLock & lock ) = 0;
			virtual void LockReadOnly( size_t bufferIndex, unify::DataLock & lock ) const = 0;
			virtual void Unlock( size_t bufferIndex, unify::DataLock & lock ) = 0;
			virtual void UnlockReadOnly( size_t bufferIndex, unify::DataLock & lock ) const = 0;

			virtual bool Locked( size_t bufferIndex ) const = 0;

			virtual me::render::BufferUsage::TYPE GetUsage( size_t bufferIndex ) const = 0;

			virtual size_t GetStride( size_t bufferIndex ) const = 0;
			virtual size_t GetSizeInBytes( size_t bufferIndex ) const = 0;
			virtual size_t GetLength( size_t bufferIndex ) const = 0;
		};
	}
}