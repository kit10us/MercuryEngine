// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <unify/Path.h>
#include <map>
#include <memory>

namespace rm
{
	/// <summary>
	/// Factory from a source path.
	/// </summary>
	template< typename T >
	class ISourceFactory
	{
	public:
		~ISourceFactory() {}
		virtual std::shared_ptr< T > Produce( unify::Path path, void * data ) = 0;
	};
}