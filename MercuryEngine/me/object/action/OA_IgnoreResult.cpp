// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/object/action/OA_IgnoreResult.h>

using namespace me;
using namespace object;
using namespace action;

IgnoreResult::IgnoreResult(IObjectAction::ptr action)
	:m_action{ action }
{
}

bool IgnoreResult::Perform( Object* object, const UpdateParams & params)
{
	if (m_action)
	{
		m_action->Perform(object, params);
	}

	return true;
}
