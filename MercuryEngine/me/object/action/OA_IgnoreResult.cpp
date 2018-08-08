// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/object/action/OA_IgnoreResult.h>

using namespace me;
using namespace object;
using namespace action;

IgnoreResult::IgnoreResult(IObjectAction::ptr action)
	:m_action{ action }
{
}

bool IgnoreResult::Perform( Object* object, float delta )
{
	if (m_action)
	{
		m_action->Perform(object, delta );
	}

	return true;
}
