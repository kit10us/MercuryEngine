// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/action/ToggleAction.h>

using namespace me;
using namespace action;

ToggleAction::ToggleAction( std::initializer_list< IAction::ptr > actions )
	: m_actions{ actions }
	, m_next{ 0 }
{
}

bool ToggleAction::Perform()
{
	if (!m_actions.empty())
	{
		m_actions[m_next]->Perform();
		m_next++;
		if (m_next >= m_actions.size())
		{
			m_next = 0;
		}
	}
	return true;
}
