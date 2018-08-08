// Copyright (c) 2006 - 2014 Evil Quail LLC
// All Rights Reserved

#pragma once

#include <string>
#include <unify/Exception.h>

namespace me
{
	namespace exception
	{
		class OutOfBounds : public unify::Exception 
		{
		public:
			OutOfBounds( std::string error )
				: unify::Exception( error )
			{
			}
		};
	}
}