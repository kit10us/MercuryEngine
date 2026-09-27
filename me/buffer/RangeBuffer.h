
#pragma once

#include <me/buffer/IBuffer.h>
#include <unify/Range.h>
#include <memory>


namespace me::buffer
{
    /// @brief
    /// A buffer that gives access to a range segment within another buffer.
    class RangeBuffer : public IBuffer
    {
    public:
        RangeBuffer();

        RangeBuffer(IBuffer::ptr buffer, unify::Range<size_t> range);

        ~RangeBuffer();

    public: // IBuffer
        /// @brief Resets the range buffer only - does not modify the member buffer itself.
        void Invalidate() override;

        /// @brief Returns the allocation status of the owned buffer.
        /// @return 
        bool IsValid() const override;

        size_t Size() const override;

        unify::Result<BufferLock> Lock(int32_t offset, std::optional<size_t> size) override;

        unify::Result<ReadOnlyBufferLock> LockReadOnly(int32_t offset, std::optional<size_t> size) const override;

    public: // RangeBuffer
        void SetBuffer(IBuffer::ptr buffer, unify::Range<size_t> range);


        IBuffer::ptr GetBuffer() const;

        unify::Range<size_t> GetRange() const;

        void Reset();

    private:
        IBuffer::ptr m_buffer;
        unify::Range<size_t> m_range;
    };
}