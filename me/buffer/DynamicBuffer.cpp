
#include <me/buffer/DynamicBuffer.h>

#include <cstring>

using namespace me;
using namespace buffer;


DynamicBuffer::DynamicBuffer()
: m_lock {}
, m_dynamic_size {}
, m_dynamic_buffer {}
{
}

DynamicBuffer::DynamicBuffer(size_t size)
: DynamicBuffer()
{
    Allocate(size);
}

DynamicBuffer::~DynamicBuffer()
{
}

void DynamicBuffer::DynamicBuffer::Invalidate()
{
}

bool DynamicBuffer::Allocate(size_t size)
{
    m_dynamic_buffer.reset(new std::byte[size]);
    m_dynamic_size = size;
    SetBuffer(m_dynamic_buffer.get(), size);
    return true;
}
