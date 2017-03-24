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
		namespace action
		{
			/// <summary>
			/// InputAction which triggers an IAction.
			/// </summary>
			class Action : public IInputAction
			{
			public:
				Action( me::action::IAction::ptr action );

				/// <summary>
				/// Perform the action.	Returns true on success.
				/// </summary>
				bool Perform( IInputDevice * device, IInputCondition* input, float delta ) override;

			private:
				me::action::IAction::ptr m_action;
			};
		}
	}
}