// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/dyna/scale/DS_Absolute.h>

using namespace me;
using namespace dyna;
using namespace scale;

Absolute::Absolute(unify::V3< float > v3)
	: m_v3{ v3 }
{
}

Result Absolute::GetScale(unify::V3< float > & out, unify::TimeDelta delta)
{
	out = m_v3;
	return Result::Success;
}
