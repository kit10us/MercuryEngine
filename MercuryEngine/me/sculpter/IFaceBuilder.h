// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/sculpter/SculptureStats.h>
#include <me/sculpter/FaceInput.h>
#include <me/sculpter/FaceOutput.h>
#include <memory>

namespace me
{
	namespace sculpter
	{
		class IFaceBuilder
		{
		public:
			typedef std::shared_ptr< IFaceBuilder > ptr;

			virtual ~IFaceBuilder() {}

			virtual FaceOutput Build( SheetStats sheetStats, FaceInput input ) = 0;
		};
	}
}