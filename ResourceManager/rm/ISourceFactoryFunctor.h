#pragma once
#include <unify/Path.h>

namespace rm
{
	template< typename T >
	class ISourceFactoryFunctor
	{
	public:
		virtual ~ISourceFactoryFunctor() {}

		virtual T * operator()( unify::Path source ) = 0;
	};
}