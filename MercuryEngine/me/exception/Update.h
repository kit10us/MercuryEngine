// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Exception.h>

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