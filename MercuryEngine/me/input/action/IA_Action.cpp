// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/input/action/IA_Action.h>

namespace me
{
	namespace input
	{
		namespace action
		{
			Action::Action( me::action::IAction::ptr action )
				: m_action{ action }
			{
			}

			bool Action::Perform( IInputDevice * device, IInputCondition* input, unify::TimeDelta delta )
			{
				// We ignore the device.
				// We ignore the input.
				// We ignore the delta.
				return m_action->Perform();
			}
		}
	}
}
