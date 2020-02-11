// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Exception.h>

namespace me
{
	namespace exception
	{
		class Handled : public unify::Exception
		{
		public:
			Handled( std::string what )
				: Exception( what )
			{
			}
		};
	}
}