// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/dyna/Dyna.h>
#include <unify/V3.h>
#include <unify/TimeDelta.h>
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

			virtual Result GetPosition( unify::V3< float > & out, unify::TimeDelta delta ) = 0;
		};
	}
}