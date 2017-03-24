// Copyright (c) 2003 - 2013, Quentin S. Smith
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

			bool Action::Perform( IInputDevice * device, IInputCondition* input, float delta )
			{
				// We ignore the device.
				// We ignore the input.
				// We ignore the delta.
				return m_action->Perform();
			}
		}
	}
}
