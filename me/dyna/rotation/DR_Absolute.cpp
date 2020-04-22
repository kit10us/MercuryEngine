// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/dyna/rotation/DR_Absolute.h>

using namespace me::dyna;
using namespace me::dyna::rotation;

Absolute::Absolute(unify::Quaternion q)
	: m_q{ q }
{
}

Result Absolute::GetRotation(unify::Quaternion & out, unify::TimeDelta delta)
{
	out = m_q;
	return Result::Success;
}
