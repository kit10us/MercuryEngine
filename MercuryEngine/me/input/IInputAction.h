// Copyright (c) 2002 - 2018, Evil Quail LLC
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
			virtual bool Perform( IInputDevice * device, IInputCondition * input, float delta ) = 0;
		};
	}
}