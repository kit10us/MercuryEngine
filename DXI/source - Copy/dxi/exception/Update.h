// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <string>
#include <exception>

namespace dxi
{
	namespace exception
	{
		class Update : public unify::Exception 
		{
		public:
			Update( const std::string & error )
				: unify::Exception( error )
			{
			}
		};
	} // namespace exception
} // namespace dxi