// Copyright (c) 2003 - 2017, Quentin S. Smith
// All Rights Reserved

#include <me/action/ObjectAction.h>

using namespace me;
using namespace action;

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
