// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/V3.h>
#include <me/dyna/Dyna.h>
#include <memory>

namespace me
{
	namespace dyna
	{
		class IDynaPosition
		{
		public:
			typedef std::shared_ptr< IDynaPosition > ptr;
			~IDynaPosition() {}

			virtual Result GetPosition( unify::V3< float > & out, float delta ) = 0;
		};
	}
}