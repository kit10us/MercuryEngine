
#include <me/buffer/StaticBuffer.h>

#include <unify/Cast.h>
#include <cstring>

using namespace me;
using namespace buffer;


StaticBuffer::StaticBuffer()
: m_mutex {}
, m_size {}
, m_buffer {}
{
}

StaticBuffer::StaticBuffer(std::byte* buffer, size_t size)
: m_mutex {}
, m_size {size}
{
    SetBuffer(buffer, size);
}

StaticBuffer::~StaticBuffer()
{
    m_buffer = nullptr;
}

void StaticBuffer::Invalidate()
{
    m_buffer = nullptr;
}

bool StaticBuffer::IsValid() const
{
    return m_buffer != nullptr;
}

size_t StaticBuffer::Size() const
{
    return m_size;
}

unify::Result<BufferLock> StaticBuffer::Lock(int32_t offset, std::optional<size_t> size)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if (size)
    {
        if ((offset + *size) > m_size)
        {
            return unify::Failure {};
        }

        return BufferLock(std::move(lock), m_buffer + offset, *size);
    }
    else
    {
        if (offset > static_cast<int32_t>(m_size))
        {
            return unify::Failure {};
        }
        return BufferLock(std::move(lock), m_buffer + offset, m_size - offset);
    }
}

unify::Result<ReadOnlyBufferLock> StaticBuffer::LockReadOnly(int32_t offset, std::optional<size_t> size) const
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if (size)
    {
        if ((offset + *size) > m_size)
        {
            return unify::Failure{};
        }

        return ReadOnlyBufferLock(std::move(lock), m_buffer + offset, *size);
    }
    else
    {
        if (offset > static_cast<int32_t>(Size()))
        {
            return unify::Failure{};
        }

        return ReadOnlyBufferLock(std::move(lock), m_buffer + offset, m_size - offset);
    }
}

void StaticBuffer::SetBuffer(std::byte* buffer, size_t size)
{
    m_buffer = buffer;
    m_size = size;
}
