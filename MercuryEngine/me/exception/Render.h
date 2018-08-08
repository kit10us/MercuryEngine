// Copyright (c) 2002 - 2018, Evil Quail LLC
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
			Render( std::string error )
				: unify::Exception( error )
			{
			}
		};
	}
}