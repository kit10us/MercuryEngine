// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <unify/V3.h>
#include <me/dyna/Dyna.h>
#include <memory>

namespace me
{
	namespace dyna
	{
		class IDynaScale
		{
		public:
			typedef std::shared_ptr< IDynaScale > ptr;
			~IDynaScale() {}

			virtual Result GetScale( unify::V3< float > & out, float delta ) = 0;
		};
	}
}