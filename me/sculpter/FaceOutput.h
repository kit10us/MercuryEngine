// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <vector>

namespace me
{
	namespace sculpter
	{
		struct FaceOutput
		{
			FaceOutput();

			size_t index;
			std::vector< size_t > indices;
		};
	}
}