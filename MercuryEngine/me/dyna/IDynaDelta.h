// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/dyna/Dyna.h>
#include <unify/TimeDelta.h>
#include <memory>

namespace me
{
	namespace dyna
	{
		class IDynaDelta
		{
		public:
			typedef std::shared_ptr< IDynaDelta > ptr;
			~IDynaDelta() {}

			virtual Result GetDelta( unify::TimeDelta & delta ) = 0;
		};
	}
}