// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Mesh.h>
#include <me/factory/EffectFactories.h>
#include <array>

namespace terrain
{
	enum class Direction : size_t
	{
		UL, U, UR,
		L, C, R,
		DL, D, DR,
		UDL, UDR,
		ULR, DLR,
		UD, LR,
		Surround
	};
	const size_t DirectionCount = 16;
}