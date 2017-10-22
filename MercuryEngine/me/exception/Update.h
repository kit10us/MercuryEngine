// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <string>
#include <exception>

namespace me
{
	namespace exception
	{
		class Update : public unify::Exception 
		{
		public:
			Update( std::string error )
				: unify::Exception( error )
			{
			}
		};
	}
}