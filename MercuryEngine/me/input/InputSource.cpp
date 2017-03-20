// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/input/InputSource.h>

#include <me/input/InputType.h>
#include <me/action/IAction.h>
#include <memory>
#include <string>

using namespace me;

using namespace input;


InputSource::InputSource()
{
}

InputSource::~InputSource() 
{
}

void InputSource::AddEvent( unify::Owner::ptr owner, IInputCondition::ptr condition, IInputAction::ptr action )
{
	m_events.push_back({ owner, condition, action });
}

void InputSource::ClearEvents()
{
	m_events.clear();
}

int InputSource::HandleEvents()
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
			if ( itr->condition->IsTrue() )
			{
				itr->action->Perform(itr->condition.get());
				triggered++;
			}
			itr++;
		}
	}
	return triggered;
}
