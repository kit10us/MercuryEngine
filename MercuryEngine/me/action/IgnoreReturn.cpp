// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/action/IgnoreReturn.h>

using namespace me;
using namespace action;

IgnoreReturn::IgnoreReturn( IAction::ptr action )
	: m_action{ action }
{
}

bool IgnoreReturn::Perform()
{
	m_action->Perform();
	return true;
}
