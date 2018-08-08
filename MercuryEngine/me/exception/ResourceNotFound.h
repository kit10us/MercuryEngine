// Copyright (c) 2006 - 2014 Evil Quail LLC
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
			ResourceNotFound( std::string name )
				: unify::Exception( "Resource \"" + name + "\" not found!" )
			{
			}
			ResourceNotFound( std::string name, std::string type )
				: unify::Exception( "Resource \"" + name + "\" of type \"" + type + "\" not found!" )
			{
			}
		};
	}
}