// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <unify/Exception.h>
#include <string>

namespace me
{
	namespace render
	{
		struct Format
		{
			enum TYPE
			{
				FloatR32G32B32A32
			};

			static TYPE FromString( std::string format );
			static std::string ToString( TYPE format );
		};
	}
}