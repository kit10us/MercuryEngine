// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/dyna/Dyna.h>
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

			virtual Result GetDelta( float & delta ) = 0;
		};
	}
}