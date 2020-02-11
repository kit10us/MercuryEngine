// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/sculpter/SheetStats.h>
#include <me/sculpter/VertexInput.h>
#include <me/sculpter/VertexOutput.h>
#include <memory>

namespace me
{
	namespace sculpter
	{
		class IVertexBuilder
		{
		public:
			typedef std::shared_ptr< IVertexBuilder > ptr;

			virtual ~IVertexBuilder() {}

			virtual VertexOutput Build( SheetStats sheetStats, VertexInput input ) = 0;
		};
	}
}