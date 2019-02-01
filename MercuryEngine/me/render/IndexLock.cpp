// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/render/IndexLock.h>

using namespace me;
using namespace render;

void IndexLock::SetIndex( unsigned int index, unsigned int value )
{
	memcpy( (unsigned char*)m_data + index * m_stride, &value, m_stride );
}

unsigned int IndexLock::GetIndex( unsigned int index )
{
	unsigned int value = 0;
	memset( &value, (int)((unsigned char*)m_data + index * m_stride), m_stride );
	return value;
}