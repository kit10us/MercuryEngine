// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/dyna/position/DP_Absolute.h>

using namespace me;
using namespace dyna;
using namespace position;

Absolute::Absolute(unify::V3< float > v3)
	: m_v3{ v3 }
{
}

Result Absolute::GetPosition(unify::V3< float > & out, float delta)
{
	out = m_v3;
	return Result::Success;
}
