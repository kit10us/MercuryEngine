// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/dyna/position/DP_Object.h>

using namespace me;
using namespace dyna;
using namespace position;

Object::Object( object::Object * object )
	: m_object{ object }
{
}

Result Object::GetPosition(unify::V3< float > & out, unify::TimeDelta delta)
{
	if (m_object)
	{
		out = m_object->GetFrame().GetPosition();
		return Result::Success;
	}

	return Result::Failure;
}
