// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/sculpter/Vertex.h>
#include <me/sculpter/VertexOutput.h>

namespace me
{
	namespace sculpter
	{
		struct VertexInput
		{
			VertexInput();

			size_t index;
			Vertex vertex;
			VertexOutput previous;
		};
	}
}