// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/exception/FailedToCreate.h>
#include <dxi/exception/FailedToLock.h>
#include <dxi/exception/OutOfBounds.h>
#include <dxi/DataBuffer.h>
#include <unify/String.h>
#include <assert.h>

using namespace dxi;

BufferUsage::TYPE BufferUsage::FromString( const std::string & usage )
{
	if ( unify::StringIs( usage, "Default" ) )
	{
		return Default;
	}
	else if ( unify::StringIs( usage, "Immutable" ) )
	{
		return Immutable;
	}
	else if ( unify::StringIs( usage, "Dynamic" ) )
	{
		return Dynamic;
	}
	else if ( unify::StringIs( usage, "Staging" ) ) 
	{
		return Staging;
	}
	else
	{
		throw unify::Exception( "BufferUsage::FromString: Not a valid usage type!" );
	}
}

std::string BufferUsage::ToString( BufferUsage::TYPE usage )
{
	switch( usage )
	{
	case Default:
		return "Default";
	case Immutable:
		return "Immutable";
	case Dynamic:
		return "Dynamic";
	case Staging:
		return "Staging";
	default:
		throw unify::Exception( "BufferUsage::ToString: Not a valid usage type!" );
	}
}

DataBuffer::DataBuffer()
: m_locked( false )
, m_usage( BufferUsage::Default )
, m_length( 0 )
, m_stride( 0 )
{
}

DataBuffer::~DataBuffer()
{
}

bool DataBuffer::Locked() const
{
	return m_locked;
}

BufferUsage::TYPE DataBuffer::GetUsage() const
{
	return m_usage;
}

unsigned int DataBuffer::GetStride() const
{
	return m_stride;
}

unsigned int DataBuffer::GetLength() const
{
	return m_length;
}

unsigned int DataBuffer::GetSize() const
{
	return m_stride * m_length;
}
