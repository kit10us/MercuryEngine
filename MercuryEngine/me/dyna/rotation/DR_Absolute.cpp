// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/dyna/rotation/DR_Absolute.h>

using namespace me;
using namespace dyna;
using namespace rotation;

Absolute::Absolute(unify::Quaternion q)
	: m_q{ q }
{
}

Result Absolute::GetRotation(unify::Quaternion & out, float delta)
{
	out = m_q;
	return Result::Success;
}
