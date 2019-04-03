// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/sculpter/SheetStats.h>
#include <me/sculpter/IVertexBuilder.h>
#include <me/sculpter/IFaceBuilder.h>

namespace me
{
	namespace sculpter
	{
		struct Sheet
		{
			Sheet();

			SheetStats stats;
			IVertexBuilder::ptr vertexBuilder;
			IFaceBuilder::ptr faceBuilder;
		};
	}
}