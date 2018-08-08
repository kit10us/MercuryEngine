// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <unify/Exception.h>

namespace me
{
	namespace exception
	{
		class NotImplemented : public unify::Exception
		{
		public:
			NotImplemented( std::string what = std::string() )
				: Exception( std::string( "Not implemented: " ) + what )
			{
			}
		};
	}
}