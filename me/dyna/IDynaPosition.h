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
		class IDynaPosition
		{
		public:
			typedef std::shared_ptr< IDynaPosition > ptr;
			virtual ~IDynaPosition() {}

			virtual Result GetPosition( unify::V3< float > & out, unify::TimeDelta delta ) = 0;
		};
	}
}