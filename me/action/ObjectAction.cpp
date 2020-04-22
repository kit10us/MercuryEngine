// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/action/ObjectAction.h>

using namespace me::action;

ObjectAction::ObjectAction( object::Object* object, object::action::IObjectAction::ptr action )
	: m_object{ object }
	, m_action{ action }
{
}

bool ObjectAction::Perform()
{
	m_action->Perform( m_object, 1.0f );
	return true;
}
