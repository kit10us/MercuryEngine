// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

namespace me
{
	namespace sculpter
	{
		struct SculptureStats
		{
			SculptureStats();

			size_t totalSheets;
			size_t totalVertices;
			size_t totalIndices;
			size_t totalTriangles;
		};
	}
}