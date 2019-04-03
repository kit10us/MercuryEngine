// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/V3.h>
#include <unify/Color.h>
#include <unify/TexCoords.h>

namespace me
{
	namespace sculpter
	{
		struct Vertex
		{
			unify::V3< float > pos;
			unify::V3< float > normal;
			unify::Color diffuse;
			unify::Color specular;
			unify::TexCoords coords;
		};
	}
}