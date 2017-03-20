// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <memory>

namespace me
{
	namespace input
	{
		class IInputCondition
		{
		public:
			typedef std::shared_ptr< IInputCondition > ptr;

			virtual ~IInputCondition() {}
			virtual bool IsTrue() const = 0;
			virtual float GetValue() const = 0;

			virtual std::shared_ptr < class IInputSource > GetSource() const = 0;
		};
	}
}