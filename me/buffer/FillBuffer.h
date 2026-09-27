
#pragma once

#include <me/buffer/IBuffer.h>
#include <unify/Result.h>


namespace me::buffer
{
    /// @brief A buffer that can be filled up and emptied out in chunks.
    /// @note once the buffer is empty it clears itself, regardless of if the buffer had gotten full.
    class FillBuffer
    {
    public:
        using ptr = std::shared_ptr<FillBuffer>;

        FillBuffer();

        FillBuffer(IBuffer::ptr buffer);

        ~FillBuffer();

        void Invalidate();

        bool IsValid() const;

        size_t Size() const;
        
        void SetBuffer(IBuffer::ptr buffer);
        
        IBuffer::ptr GetBuffer();


        /// @brief Lock buffer for writting.
        /// @note Will need to AddWritten to let the fill buffer know the bytes have been written to.
        /// @return a lock on success.
        unify::Result<BufferLock> LockWrite();

        /// @brief Lock buffer for writting.
        /// @note Will need to RemoveRead to let the fill buffer know the bytes have been read from.
        /// @return a lock on success
        unify::Result<ReadOnlyBufferLock> LockRead();
       
       /// @brief Add written bytes having used LockWrite.
       /// @param size bytes written
       /// @return \c true if we were able to read size bytes
       bool AddWritten(size_t size);

        /// @brief Remove read bytes having used LockRead.
        /// @param size bytes read
        /// @return \c true if we were able to write size bytes
        bool RemoveRead(size_t size, bool clear_on_empty);
       
        /// @brief Clear buffer to empty.
        void Clear();

        /// @brief Amount of free space in the buffer that can be filled.
        /// @return bytes free space
        size_t BytesFree() const;

        /// @brief Amount of bytes the buffer has been filled.
        /// @return bytes filled
        size_t BytesFilled() const;

        /// @brief Amout of bytes that have been read since Clear.
        /// @return bytes read
        size_t BytesRead() const;
        
        /// @brief Check if the buffer is full.
        /// @return \c true if full
        bool Full() const;

        /// @brief Check if the buffer is empty.
        /// @return \c true if empty
        bool Empty() const;

    private:
        IBuffer::ptr m_buffer;
        size_t m_front;
        size_t m_middle;
        size_t m_back;
    };
}
