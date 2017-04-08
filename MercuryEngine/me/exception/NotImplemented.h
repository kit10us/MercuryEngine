// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <unify/Exception.h>

namespace me
{
	namespace exception
	{
		class NotImplemented : public unify::Exception
		{
		public:
			NotImplemented( const std::string & what = std::string() )
				: Exception( std::string( "Not implemented: " ) + what )
			{
			}
		};
	}
}