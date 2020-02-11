// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/sculpter/FaceOutput.h>
#include <vector>

namespace me
{
	namespace sculpter
	{
		struct FaceInput
		{
			FaceInput();

			size_t index;
			std::vector< size_t > indices;
			FaceOutput previous;
		};
	}
}