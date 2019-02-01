// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/action/ActionList.h>

using namespace me;
using namespace action;

ActionList::ActionList()
{
}

void ActionList::AddAction( IAction::ptr action )
{
	m_actions.push_back( action );
}

bool ActionList::Perform()
{
	for ( auto && action : m_actions )
	{
		if ( !action->Perform() )
		{
			return false;
		}
	}
	return true;
}
