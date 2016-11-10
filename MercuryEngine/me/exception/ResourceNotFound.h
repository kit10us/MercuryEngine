// Copyright (c) 2006 - 2014 Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/exception/Exception.h>
#include <string>

namespace me
{
	namespace exception
	{
		class ResourceNotFound : public unify::Exception 
		{
		public:
			ResourceNotFound( const std::string & name )
				: unify::Exception( "Resource \"" + name + "\" not found!" )
			{
			}
			ResourceNotFound( const std::string & name, const std::string & type )
				: unify::Exception( "Resource \"" + name + "\" of type \"" + type + "\" not found!" )
			{
			}
		};
	}
}