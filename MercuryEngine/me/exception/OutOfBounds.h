// Copyright (c) 2006 - 2014 Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Exception.h>

namespace me
{
	namespace exception
	{
		class OutOfBounds : public unify::Exception 
		{
		public:
			OutOfBounds( std::string error )
				: unify::Exception( error )
			{
			}
		};
	}
}