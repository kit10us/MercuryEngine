// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <string>
#include <exception>

namespace me
{
	namespace exception
	{
		class Startup : public unify::Exception 
		{
		public:
			Startup( std::string error )
				: unify::Exception( error )
			{
			}
		};
	} // namespace exception
} // namespace dxi