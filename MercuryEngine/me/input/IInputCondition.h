// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/V3.h>
#include <memory>

namespace me
{
	namespace input
	{
		class IInputDevice;

		class IInputCondition
		{
		public:
			typedef std::shared_ptr< IInputCondition > ptr;

			virtual ~IInputCondition() {}
			virtual bool IsTrue( IInputDevice* device ) const = 0;
			virtual unify::V3< float > GetValue( IInputDevice* device  ) const = 0;
		};
	}
}