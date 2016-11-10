// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/DataBuffer.h>
#include <memory>

namespace me 
{
	typedef unsigned long Index32;

	struct IndexBufferParameters
	{
		IndexBufferParameters()
			: numIndices{}
			, source{}
			, usage{ me::BufferUsage::Default }
			, flags {}
		{
		}

		IndexBufferParameters( unsigned int _count, const Index32 * _source, me::BufferUsage::TYPE _usage = me::BufferUsage::Default, unify::Flags _flags = 0 )
			: numIndices{ _count }
			, source{ _source }
			, usage{ _usage }
			, flags{ _flags }
		{
		}

		unsigned int numIndices;
		const Index32 * source;
		BufferUsage::TYPE usage;
		unify::Flags flags;
	};

	class IIndexBuffer
	{
	public:
		typedef std::shared_ptr< IIndexBuffer > ptr;

		virtual ~IIndexBuffer() {}

		virtual void Create( IndexBufferParameters parameters ) = 0;
		virtual void Use() const = 0;
	};
}