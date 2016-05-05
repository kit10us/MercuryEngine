// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <string>
#include <exception>

namespace rm
{
	namespace exception
	{
		class FailedToCreate : public std::exception
		{
		public:
			FailedToCreate( const std::string & error )
				: std::exception( error.c_str() )
			{
			}
		};
	}
}