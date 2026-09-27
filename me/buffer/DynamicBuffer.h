
#pragma once

#include <me/buffer/StaticBuffer.h>

#include <memory>


namespace me::buffer
{
    /// @brief
    /// A buffer that manages a memory block.
    class DynamicBuffer : public StaticBuffer
    {
    public:
        DynamicBuffer();
        DynamicBuffer(size_t size);
        
        virtual ~DynamicBuffer();

    public: // IBuffer overrides
        void Invalidate() override;
        
    public:
        /// @brief Allocate buffer memory.
        /// @param size bytes to allocate
        /// @return true if allocation succeeded
        bool Allocate(size_t size);

    private:
        mutable std::mutex m_lock;
        size_t m_dynamic_size;
        std::shared_ptr<std::byte[]> m_dynamic_buffer;
    };
}