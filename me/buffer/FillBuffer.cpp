
#include <me/buffer/FillBuffer.h>

#include <iostream>

using namespace me;
using namespace buffer;

FillBuffer::FillBuffer()
: m_buffer {}
, m_front {}
, m_middle {}
{
}

FillBuffer::FillBuffer(IBuffer::ptr buffer)
: m_buffer {buffer}
, m_front {}
, m_middle {}
{
}

FillBuffer::~FillBuffer()
{
}

void FillBuffer::Invalidate()
{
    m_buffer.reset();
}

bool FillBuffer::IsValid() const
{
    return m_buffer && m_buffer->IsValid();
}

size_t FillBuffer::Size() const
{
    return m_buffer ? m_buffer->Size() : 0;
}

void FillBuffer::SetBuffer(IBuffer::ptr buffer)
{
    m_buffer = buffer;
}

IBuffer::ptr FillBuffer::GetBuffer()
{
    return m_buffer;
}

unify::Result<BufferLock> FillBuffer::LockWrite()
{
    if (!m_buffer)
    {
        return unify::Failure{"Fill buffer is not valid!"};
    }

    return m_buffer->Lock(static_cast<int32_t>(m_middle), BytesFree());
}

unify::Result<ReadOnlyBufferLock> FillBuffer::LockRead()
{
    if (!m_buffer)
    {
        return unify::Failure{"Fill buffer is invalid!"};
    }

    return m_buffer->LockReadOnly(static_cast<int32_t>(m_front), BytesFilled());
}

bool FillBuffer::AddWritten(size_t size)
{
    if (size > BytesFree())
    {
        return false;
    }

    m_middle += size;

    return true;
}

bool FillBuffer::RemoveRead(size_t size, bool clear_on_empty)
{
    if (size > BytesFilled())
    {
        return false;
    }

    m_front += size;

    if (clear_on_empty && m_front == m_middle)
    {
        Clear();
    }

    return true;
}
    
void FillBuffer::Clear()
{
    m_front = 0;
    m_middle = 0;
}

size_t FillBuffer::BytesFree() const
{
    return Size() - m_middle;
}

size_t FillBuffer::BytesFilled() const
{
    return m_middle - m_front;
}

size_t FillBuffer::BytesRead() const
{
    return m_front;
}

bool FillBuffer::Full() const
{
    return m_front == 0  && m_middle == Size();
}

bool FillBuffer::Empty() const
{
    return m_front == m_middle;
}
