// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/input/action/IA_ObjectAction.h>

namespace me
{
	namespace input
	{
		namespace action
		{
			ObjectAction::ObjectAction( me::object::Object * object, me::object::action::IObjectAction::ptr action )
				: m_object{ object }
				, m_action{ action }
			{
			}

			bool ObjectAction::Perform( IInputDevice * device, IInputCondition* input, float delta )
			{
				// We ignore the device.
				// We ignore the input.
				// We ignore the delta.
				return m_action->Perform( m_object, delta );
			}
		}
	}
}
