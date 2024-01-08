// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/render/IndexLock.h>

using namespace me;
using namespace render;

void IndexLock::SetIndex( size_t index, size_t value )
{
	memcpy( (unsigned char*)m_data + index * m_stride, &value, m_stride );
}

size_t IndexLock::GetIndex( size_t index )
{
	size_t value = 0;
	memset( (unsigned char*)&value, int((intptr_t)m_data) + (int)(index * m_stride), m_stride );
	return value;
}