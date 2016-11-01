// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <string>

namespace dxi
{
	struct Filtering
	{
		enum TYPE
		{
			Point, Linear, Anisotropic
		};

		static std::string ToString( TYPE filtering );
		static TYPE FromString( std::string filtering );
	};
}