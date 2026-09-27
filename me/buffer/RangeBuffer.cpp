
#include <me/buffer/RangeBuffer.h>
#include <unify/Cast.h>

#include <cstring>

using namespace me;
using namespace buffer;


RangeBuffer::RangeBuffer()
: m_buffer {}
, m_range {}
{
}

RangeBuffer::RangeBuffer(IBuffer::ptr buffer, unify::Range<size_t> range)
: m_buffer {buffer}
, m_range {range}
{
}

RangeBuffer::~RangeBuffer()
{
}

void RangeBuffer::Invalidate()
{
    m_buffer.reset();
}

bool RangeBuffer::IsValid() const
{
    return m_buffer && m_buffer->IsValid() && m_range.Max() <= m_buffer->Size();
}

size_t RangeBuffer::Size() const
{
    if (!IsValid())
    {
        return 0;
    }

    return m_range.Size();
}

unify::Result<BufferLock> RangeBuffer::Lock(int32_t offset, std::optional<size_t> size)
{
    if (!IsValid())
    {
        return unify::Failure{"Range buffer is invalid!"};
    }

    if (!size)
    {
        return m_buffer->Lock(static_cast<int32_t>(m_range.Min()), m_range.Size());
    }
    else
    {
        if (offset + *size > m_range.Max())
        {
            using namespace std;
            return unify::Failure{"Range buffer out of bounds! (offset: " + to_string(offset) + ", size: " + to_string(*size) + ", range: {" + unify::ToString(m_range) + "})"};
        }

        return m_buffer->Lock(static_cast<int32_t>(m_range.Min()) + offset, *size);
    }
}

unify::Result<ReadOnlyBufferLock> RangeBuffer::LockReadOnly(int32_t offset, std::optional<size_t> size) const
{
    if (!IsValid())
    {
        return unify::Failure("Buffer is not valid!");
    }

    if (!size)
    {
        return m_buffer->LockReadOnly(static_cast<int32_t>(m_range.Min()), m_range.Size());
    }
    else
    {
        if (offset + *size > m_range.Max())
        {
            return unify::Failure{"Buffer lock is out of range!"};
        }

        return m_buffer->LockReadOnly(static_cast<int32_t>(m_range.Min()) + offset, *size);
    }
}

void RangeBuffer::SetBuffer(IBuffer::ptr buffer, unify::Range<size_t> range)
{
    m_buffer = buffer;
    m_range = range;
}

IBuffer::ptr RangeBuffer::GetBuffer() const
{
    return m_buffer;
}

unify::Range<size_t> RangeBuffer::GetRange() const
{
    return m_range;
}

void RangeBuffer::Reset()
{
    m_buffer.reset();
    m_range = {};
}
