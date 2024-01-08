// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/render/IIndexBuffer.h>
#include <vector>

namespace me
{
	namespace sculpter
	{
		struct FaceOutput
		{
			FaceOutput();

			render::Index32 index;
			std::vector< me::render::Index32 > indices;
		};
	}
}