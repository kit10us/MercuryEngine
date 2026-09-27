
#pragma once

#include <optional>
#include <cstddef>

#include <me/buffer/IBuffer.h>


namespace me::buffer
{
    /// @brief
    /// A buffer that points to an unmanaged/static memory block.
    class StaticBuffer : public IBuffer
    {
    public:
        StaticBuffer();
        StaticBuffer(std::byte* buffer, size_t size);
        
        virtual ~StaticBuffer();

    public: // IBuffer overrides
        void Invalidate() override;

        bool IsValid() const override;

        size_t Size() const override;

        unify::Result<BufferLock> Lock(int32_t offset = 0, std::optional<size_t> size = {}) override;
    
        unify::Result<ReadOnlyBufferLock> LockReadOnly(int32_t offset = 0, std::optional<size_t> size = {}) const override;
    
    public: // StaticBuffer
        /// @brief Set the block of memory for the buffer.
        /// @param buffer block of memory pointer to
        /// @param size size of the buffer
        void SetBuffer(std::byte* buffer, size_t size);

    private:
        mutable std::mutex m_mutex;
        size_t m_size;
        std::byte* m_buffer; // We do not manage this memory block.
    };
}