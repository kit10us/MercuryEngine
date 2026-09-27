
#include <me/buffer/BufferLock.h>

#include <string.h>

#include <iostream>

using namespace me;
using namespace buffer;

ReadOnlyBufferLock::ReadOnlyBufferLock(ReadOnlyBufferLock&& lock)
: m_lock {std::move(lock.m_lock)}
, m_buffer {lock.m_buffer}
, m_size {lock.m_size}
{
}

ReadOnlyBufferLock::ReadOnlyBufferLock(std::unique_lock<std::mutex>&& lock, std::byte* buffer, size_t size)
: m_lock {std::move(lock)}
, m_buffer {buffer}
, m_size {size}
{
}

ReadOnlyBufferLock::~ReadOnlyBufferLock()
{
    m_buffer = nullptr;
    m_size = 0;
}

size_t ReadOnlyBufferLock::Size() const
{
    return m_size;
}

const std::byte* ReadOnlyBufferLock::GetBuffer() const
{
    return m_buffer;
}

const std::byte* ReadOnlyBufferLock::operator*() const
{
    return m_buffer;
}

const std::byte ReadOnlyBufferLock::operator[](int32_t offset) const
{
    if (offset < 0 || offset >= (int32_t)m_size)
    {
        throw std::out_of_range("Attempted to access memory out of range!");
    }
    
    return m_buffer[offset];
}

bool ReadOnlyBufferLock::CopyTo(std::byte* dest_buffer, size_t size, int32_t from_offset) const
{
    if ((from_offset + size) > Size())
    {
        return false;
    }

    memcpy(dest_buffer, m_buffer, size);
    return true;
}


BufferLock::BufferLock(BufferLock&& lock)
: ReadOnlyBufferLock(std::move(lock.m_lock), lock.m_buffer, lock.m_size)
{
}

BufferLock::BufferLock(std::unique_lock<std::mutex>&& lock, std::byte* buffer, size_t size)
: ReadOnlyBufferLock(std::move(lock), buffer, size)
{
}

std::byte* BufferLock::GetBuffer()
{
    return m_buffer;
}

std::byte* BufferLock::operator*()
{
    return m_buffer;
}

bool BufferLock::CopyFrom(const std::byte* source_buffer, size_t size, int32_t to_offset)
{
    if ((to_offset + size) > Size())
    {
        return false;
    }

    memcpy(m_buffer + to_offset, source_buffer, size);
    return true;
}

unify::Result<size_t> BufferLock::CopyFrom(const ReadOnlyBufferLock& source_buffer)
{
    const size_t bytes_to_copy = std::min<size_t>(Size(), source_buffer.Size());
    if (bytes_to_copy == 0)
    {
        return 0;
    }

    auto result = CopyFrom(*source_buffer, bytes_to_copy);
    if (!result)
    {
        return unify::Failure{};
    }
    else
    {
        return bytes_to_copy;
    }
}

bool BufferLock::CopyFrom(const ReadOnlyBufferLock& source_buffer, size_t size, int32_t from_offset, int32_t to_offset)
{
    if ((to_offset + size) > Size())
    {
        return false;
    }

    if ((from_offset + size) > source_buffer.Size())
    {
        return false;
    }

    if (size > Size())
    {
        return false;
    }

    auto result = CopyFrom(*source_buffer + from_offset, size, to_offset);
    return result;
}

unify::Result<size_t> BufferLock::Fill(std::byte byte, int32_t start, size_t length)
{
    if (!length || !Size() || (start + length) > Size())
    {
        return false;
    }

    memset(m_buffer + start, static_cast<int>(byte), length);
    return length;
}

unify::Result<size_t> BufferLock::Fill(std::byte byte)
{
    if (!Size())
    {
        return false;
    }

    return Fill(byte, 0, Size());
}
