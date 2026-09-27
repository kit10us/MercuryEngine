// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/buffer/BufferLock.h>

#include <string>
#include <memory>
#include <mutex>
#include <optional>


namespace me::buffer
{
    /// @brief 
    /// A buffer interface, providing access to memory resources.
    class IBuffer
    {
    public:
        typedef std::shared_ptr<IBuffer> ptr;
        typedef std::weak_ptr<IBuffer> weak_ptr;

        virtual ~IBuffer() = default;

        /// @brief Invalidate the buffer, such as releasing handles, destroying memory, etc.
        virtual void Invalidate() = 0;

        /// @brief
        /// Check if the buffer is in a ready state (such as allocated).
        /// @return true if valid
        virtual bool IsValid() const = 0;

        /// @brief 
        /// Get the size of the buffer.
        /// @return size
        virtual size_t Size() const = 0;

        /// @brief 
        /// Lock the buffer.
        /// @return buffer lock
        virtual unify::Result<BufferLock> Lock(int32_t offset = 0, std::optional<size_t> size = {}) = 0;

        /// @brief 
        /// Lock the buffer for read-only access.
        /// @return buffer lock
        virtual unify::Result<ReadOnlyBufferLock> LockReadOnly(int32_t offset = 0, std::optional<size_t> size = {}) const = 0;
    };
}