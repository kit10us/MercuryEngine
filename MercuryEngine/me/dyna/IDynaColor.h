// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/ColorUnit.h>
#include <me/dyna/Dyna.h>
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

			virtual Result GetColor( unify::ColorUnit & out, float delta ) = 0;
		};
	}
}