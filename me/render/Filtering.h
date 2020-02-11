// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <string>

namespace me
{
	namespace render
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
}