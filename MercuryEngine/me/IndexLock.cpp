// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <me/IndexLock.h>

using namespace me;

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