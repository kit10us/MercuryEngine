// Copyright (c) 2002 - 2018, Quentin S. Smith
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
			FailedToLock( std::string error )
				: unify::Exception( error )
			{
			}
		};
	}
}