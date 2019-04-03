// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Parameters.h>

namespace me
{
	namespace sculpter
	{
		struct SheetStats
		{
			SheetStats();

			size_t index;
			size_t totalVertices;
			size_t totalIndices;
			size_t totalFaces;
			size_t totalTriangles;
			unify::Parameters parameters;
		};
	}
}