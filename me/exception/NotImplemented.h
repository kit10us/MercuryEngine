// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

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