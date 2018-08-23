// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/dyna/Dyna.h>
#include <unify/Quaternion.h>
#include <unify/TimeDelta.h>
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

			virtual Result GetRotation( unify::Quaternion & out, unify::TimeDelta delta ) = 0;
		};
	}
}