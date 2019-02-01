// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/input/InputDevice.h>
#include <me/input/InputType.h>
#include <me/action/IAction.h>
#include <memory>
#include <string>

using namespace me;

using namespace input;


InputDevice::InputDevice()
{
}

InputDevice::~InputDevice() 
{
}

void InputDevice::AddEvent( unify::Owner::ptr owner, IInputCondition::ptr condition, IInputAction::ptr action )
{
	m_events.push_back({ owner, condition, action });
}

void InputDevice::ClearEvents()
{
	m_events.clear();
}

int InputDevice::HandleEvents( unify::TimeDelta delta )
{
	int triggered = 0;
	for( auto itr = m_events.begin(); itr != m_events.end(); )
	{
		// Ensure owner is still valid.
		if ( itr->owner.expired() )
		{
			// Remove event, if it's no longer valid.
			itr = m_events.erase( itr );
		}
		else
		{
			if ( itr->condition->IsTrue( this ) )
			{
				itr->action->Perform( this, itr->condition.get(), delta );
				triggered++;
			}
			itr++;
		}
	}
	return triggered;
}
