// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <string>

namespace dxi
{
	// Usage depends on the instance.
	struct Usage
	{
		enum TYPE
		{
			Ignore	= -1, // Do nothing: for example, when a previous usage set and we want to keep using it (optimizations).
			False	= 0, // In the case where something needs to be explicitly off.
			True	= 1, // True, use, set, modify.
			TrueNoSet = 2 // In the case where we want to use a default - like setting a texture to 0.
		};

        static TYPE FromString( const std::string text );
	};
}