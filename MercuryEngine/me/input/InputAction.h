// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/IInputCondition.h>
#include <me/input/IInputAction.h>
#include <me/action/IAction.h>
#include <memory>

namespace me
{
	namespace input
	{
		/// <summary>
		/// IInputAction which triggers an IAction.
		/// </summary>
		class InputAction : public IInputAction
		{
		public:
			InputAction( action::IAction::ptr action ) : m_action{ action } {}

			/// <summary>
			/// Perform the action.	Returns true on success.
			/// </summary>
			bool Perform( IInputCondition* input ) override
			{
				// We ignore the input.
				return m_action->Perform();
			}

		private:
			action::IAction::ptr m_action;
		};
	}
}