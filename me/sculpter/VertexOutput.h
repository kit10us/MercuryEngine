// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/sculpter/Vertex.h>

namespace me
{
	namespace sculpter
	{
		struct VertexOutput
		{
			VertexOutput();

			size_t index;
			Vertex vertex;
		};
	}
}