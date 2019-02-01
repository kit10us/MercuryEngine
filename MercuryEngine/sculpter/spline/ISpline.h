// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <sculpter/Point.h>
#include <memory>

namespace sculpter
{
	namespace spline
	{
		class ISpline
		{
		public:
			typedef std::shared_ptr< ISpline > ptr;
			virtual ~ISpline() {}

			virtual Point GetPoint( float delta ) = 0;
		};
	}
}