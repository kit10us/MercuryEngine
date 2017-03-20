// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/IInputCondition.h>
#include <memory>

namespace me
{
	namespace input
	{
		/// <summary>
		/// Provides action support in response to an IInputCondition.
		/// </summary>
		class IInputAction
		{
		public:
			typedef std::shared_ptr< IInputAction > ptr;

			/// <summary>
			/// Perform the action.	Returns true on success.
			/// </summary>
			virtual bool Perform( IInputCondition * input ) = 0;
		};
	}
}