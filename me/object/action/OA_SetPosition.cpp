// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/object/action/OA_SetPosition.h>

using namespace me;
using namespace object;
using namespace action;

SetPosition::SetPosition( dyna::IDynaPosition::ptr position )
	: m_position{ position }
{
}

bool SetPosition::Perform( Object* object, unify::TimeDelta delta )
{
	if (m_position)
	{
		unify::V3< float > position;
		dyna::Result result = m_position->GetPosition( position, delta );
		if (result == dyna::Result::Success )
		{
			object->GetFrame().SetPosition( position );
		}
		else
		{
			return false;
		}
	}

	return true;
}
