// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/object/action/OA_List.h>

using namespace me;
using namespace object;
using namespace action;

List::List(std::initializer_list< IObjectAction::ptr > actions)
	: m_actions{ actions }
{
}

bool List::Perform( Object* object, float delta )
{
	for (auto action : m_actions)
	{
		bool result = action->Perform( object, delta );
		if (!result)
		{
			return false;
		}
	}

	return true;
}
