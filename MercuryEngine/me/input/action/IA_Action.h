// Copyright (c) 2002 - 2018, Evil Quail LLC
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
				bool Perform( IInputDevice * device, IInputCondition* input, unify::TimeDelta delta ) override;

			private:
				me::action::IAction::ptr m_action;
			};
		}
	}
}