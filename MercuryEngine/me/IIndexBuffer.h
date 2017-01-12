// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/BufferUsage.h>
#include <vector>
#include <memory>

namespace me 
{
	typedef unsigned long Index32;

	struct IndexBufferParameters
	{
		IndexBufferParameters()
			: usage{ me::BufferUsage::Default }
			, flags {}
		{
		}

		IndexBufferParameters( std::vector< CountAndSource > countAndSource, me::BufferUsage::TYPE _usage = me::BufferUsage::Default, unify::Flags _flags = 0 )
			: countAndSource{ countAndSource }
			, usage{ _usage }
			, flags{ _flags }
		{
		}

		std::vector< CountAndSource > countAndSource;
		BufferUsage::TYPE usage;
		unify::Flags flags;
	};

	class IIndexBuffer : public IBuffer
	{
	public:
		typedef std::shared_ptr< IIndexBuffer > ptr;

		virtual void Create( IndexBufferParameters parameters ) = 0;
	};
}