// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/V3.h>
#include <memory>

namespace me
{
	namespace input
	{
		class IInputDevice;

		/// <summary>
		/// Defines a condition that lets us test against an input device, so we can test if we want to respond to this condition.
		/// </summary>
		class IInputCondition
		{
		public:
			typedef std::shared_ptr< IInputCondition > ptr;

			virtual ~IInputCondition() {}
			virtual std::string GetName() const = 0;
			virtual bool IsTrue( IInputDevice* device ) const = 0;
			virtual unify::V3< float > GetValue( IInputDevice* device  ) const = 0;
		};
	}
}