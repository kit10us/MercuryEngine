// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Exception.h>

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