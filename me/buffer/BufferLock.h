
#pragma once

#include <unify/Result.h>

#include <string>
#include <memory>
#include <mutex>


namespace me::buffer
{
    /// @brief 
    /// A buffer lock provides thread safe buffer access.
    class ReadOnlyBufferLock
    {
    public:
        ReadOnlyBufferLock(const ReadOnlyBufferLock&) = delete;
        ReadOnlyBufferLock& operator=(const ReadOnlyBufferLock&) = delete;

        ReadOnlyBufferLock(ReadOnlyBufferLock&& lock);

        /// @brief Create a buffer lock.
        /// @param lock mutex
        /// @param buffer buffer pointer
        /// @param size buffer size
        ReadOnlyBufferLock(std::unique_lock<std::mutex>&& lock, std::byte* buffer, size_t size);

        virtual ~ReadOnlyBufferLock();

        ReadOnlyBufferLock& operator=(ReadOnlyBufferLock&) = delete;

        /// @brief Get the size of the buffer.
        /// @return buffer size.
        size_t Size() const;
        
        /// @brief Get the buffer raw data.
        /// @throws logic_error On attempting to access the buffer when it is not locked.
        /// @return buffer pointer
        virtual const std::byte* GetBuffer() const;

        /// @brief Get the buffer raw data.
        /// @throws logic_error On attempting to access the buffer when it is not locked.
        /// @return buffer pointer
        virtual const std::byte* operator*() const;

        /// @brief Get a byte from the buffer.
        /// @param offset offset of byte
        /// @throws logic_error On attempting to access the buffer when it is not locked.
        /// @throws out_of_range When offset is out of range.
        /// @return byte
        virtual const std::byte operator[](int32_t offset) const;
        
        /// @brief
        /// Copy to a destination buffer.
        /// @note Uses a buffer lock.
        /// @param dest_buffer destination buffer
        /// @param size bytes to copy
        /// @param from_offset offset within source buffer to start copying from
        /// @return true on success
        virtual bool CopyTo(std::byte* dest_buffer, size_t size, int32_t from_offset = 0) const;

    protected:
        std::unique_lock<std::mutex> m_lock;
        std::byte* m_buffer;        
        size_t m_size;
    };

    
    /// @brief 
    /// A buffer lock provides thread safe buffer access.
    class BufferLock : public ReadOnlyBufferLock
    {
    public:
        BufferLock(BufferLock&& lock);

        /// @brief Create a buffer lock.
        /// @param lock mutex
        /// @param buffer buffer pointer
        /// @param size buffer size
        BufferLock(std::unique_lock<std::mutex>&& lock, std::byte* buffer, size_t size);

        BufferLock& operator=(BufferLock&) = delete;

        /// @brief Get the buffer raw data.
        /// @throws logic_error On attempting to access the buffer when it is not locked.
        /// @return buffer pointer
        std::byte* GetBuffer();
        
        /// @brief Get the buffer raw data.
        /// @throws logic_error On attempting to access the buffer when it is not locked.
        /// @return buffer pointer
        std::byte* operator*();

        /// @brief
        /// Copy from a source buffer.
        /// @note Uses a buffer lock.
        /// @param source_buffer source buffer
        /// @param size bytes to copy
        /// @param to_offset offset within our buffer to start copying to
        /// @return true on success
        bool CopyFrom(const std::byte* source_buffer, size_t size, int32_t to_offset = 0);

        /// @brief
        /// Copy from a source buffer.
        /// @note Uses a buffer lock. Defaults to maximum amount of bytes that can be copied.
        /// @param source_buffer source buffer
        /// @return number of bytes copied
        unify::Result<size_t> CopyFrom(const ReadOnlyBufferLock& source_buffer);

        /// @brief  
        /// Copy from a source buffer.
        /// @note Uses a buffer lock.
        /// @param source_buffer source buffer
        /// @param size bytes to copy
        /// @param from_offset offset within source buffer to start copying from
        /// @param to_offset offset within our buffer to start copying to
        /// @return true on success
        bool CopyFrom(const ReadOnlyBufferLock& source_buffer, size_t size, int32_t from_offset = 0, int32_t to_offset = 0);

        /// @brief
        /// Fill a buffer portion of the buffer with a byte.
        /// @param byte byte to fill
        /// @param start start offset to fill from
        /// @param length length of bytes to fill from start
        /// @return true on success
        unify::Result<size_t> Fill(std::byte byte, int32_t start, size_t length);

        /// @brief
        /// Fill an entire buffer with a byte.
        /// @param byte byte to fill
        /// @return true on success
        unify::Result<size_t> Fill(std::byte byte);
    };
}
