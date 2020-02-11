// Copyright (c) 2002 - 2018, Kit10 Studios LLC
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


		/// <summary>
		/// A collection of buffers of a particular buffer instance. For example, if we have a buffer for a texture, this would hold all necessary buffers internally to contribute to the entirty of the texture.
		/// </summary>
		class IBuffer
		{
		public:
			typedef std::shared_ptr< IBuffer > ptr;
						
			virtual ~IBuffer() {}

			/// <summary>
			/// Delete all buffer data, keeping parameters.
			/// </summary>
			virtual void Destroy() = 0;

			///
			/// Get the number of internal buffers.
			/// </summary>
			virtual size_t GetBufferCount() const = 0;

			/// <summary>
			/// Perform and action required to use the buffer, such as setting it as an active render resource.
			/// </summary>
			virtual void Use( size_t startBuffer = 0, size_t startSlot = 0 ) const = 0;

			/// <summary>
			/// Lock a sub-buffer.
			/// </sumamry>
			virtual void Lock( size_t bufferIndex, unify::DataLock & lock ) = 0;

			/// <summary>
			/// Unlock a sub-buffer for read-only access.
			/// </sumamry>
			virtual void LockReadOnly( size_t bufferIndex, unify::DataLock & lock ) const = 0;
			
			/// <summary>
			/// Unlock a sub-buffer.
			/// </summary>
			virtual void Unlock( size_t bufferIndex, unify::DataLock & lock ) = 0;

			/// <summary>
			/// Unlock a sub-buffer currently locked for read-only access.
			/// </summary>
			virtual void UnlockReadOnly( size_t bufferIndex, unify::DataLock & lock ) const = 0;

			/// <summary>
			/// Returns true of a specific sub-buffer is locked, else false.
			/// </summary>
			virtual bool Locked( size_t bufferIndex ) const = 0;

			/// <summary>
			/// Get a sub-buffers usage.
			/// </summary>
			virtual me::render::BufferUsage::TYPE GetUsage( size_t bufferIndex ) const = 0;
		};
	}
}