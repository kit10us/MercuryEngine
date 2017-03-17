// Copyright (c) 2003 - 2017, Quentin S. Smith
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
