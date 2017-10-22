// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <string>
#include <unify/Exception.h>

namespace me
{
	namespace exception
	{
		class FailedToCreate : public unify::Exception 
		{
		public:
			FailedToCreate( std::string error )
				: unify::Exception( error.c_str() )
			{
			}
		};
	}
}