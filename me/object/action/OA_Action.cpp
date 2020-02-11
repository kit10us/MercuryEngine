// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/object/action/OA_Action.h>

using namespace me;

object::action::Action::Action( me::action::IAction::ptr action )
	: m_action{ action }
{
}

bool object::action::Action::Perform( me::object::Object* ob, unify::TimeDelta delta )
{
	if( m_action )
	{
		return m_action->Perform();
	}

	return true;
}