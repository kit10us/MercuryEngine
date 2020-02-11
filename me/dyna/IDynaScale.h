// Copyright (c) 2002 - 2018, Kit10 Studios LLC
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
		class IDynaScale
		{
		public:
			typedef std::shared_ptr< IDynaScale > ptr;
			~IDynaScale() {}

			virtual Result GetScale( unify::V3< float > & out, unify::TimeDelta delta ) = 0;
		};
	}
}