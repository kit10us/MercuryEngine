// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <exception>
#include <string>

namespace sculpter
{
	namespace exception
	{
		class FailedToCreate : public std::exception
		{
		public:
			FailedToCreate( std::string message )
				: std::exception( message.c_str() )
			{
			}
		};
	}
}