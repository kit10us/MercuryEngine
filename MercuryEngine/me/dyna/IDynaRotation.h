// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <unify/Quaternion.h>
#include <me/dyna/Dyna.h>
#include <memory>

namespace me
{
	namespace dyna
	{
		class IDynaRotation
		{
		public:
			typedef std::shared_ptr< IDynaRotation > ptr;
			~IDynaRotation() {}

			virtual Result GetRotation( unify::Quaternion & out, float delta ) = 0;
		};
	}
}