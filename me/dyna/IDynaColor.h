// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/Dyna.h>
#include <unify/ColorUnit.h>
#include <unify/TimeDelta.h>
#include <memory>

namespace me
{
	namespace dyna
	{
		class IDynaColor
		{
		public:
			typedef std::shared_ptr< IDynaColor > ptr;
			~IDynaColor() {}

			virtual Result GetColor( unify::ColorUnit & out, unify::TimeDelta delta ) = 0;
		};
	}
}