// Copyright (c) 2003 - 2017, Quentin S. Smith
// All Rights Reserved

#include <me/action/RepeatAction.h>

using namespace me;
using namespace action;

RepeatAction::RepeatAction( IAction::ptr action, size_t times )
	: m_action{ action }
	, m_times{ times }
{
}

bool RepeatAction::Perform()
{
	for( size_t i = 0; i < m_times; i++ )
	{
		if ( ! m_action->Perform() )
		{
			return false;
		}
	}
	return true;
}