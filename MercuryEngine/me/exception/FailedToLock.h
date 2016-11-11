// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <string>
#include <exception>

namespace me
{
	namespace exception
	{
		class FailedToLock : public unify::Exception 
		{
		public:
			FailedToLock( const std::string & error )
				: unify::Exception( error )
			{
			}
		};
	}
}