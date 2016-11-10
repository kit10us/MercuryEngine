// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <string>
#include <exception>

namespace me
{
	namespace exception
	{
		class Render : public unify::Exception 
		{
		public:
			Render( const std::string & error )
				: unify::Exception( error )
			{
			}
		};
	}
}