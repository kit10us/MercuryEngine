// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/dyna/delta/DD_Absolute.h>

using namespace me::dyna::delta;
using namespace me::dyna;

Absolute::Absolute( unify::TimeDelta delta )
	: m_delta{ delta }
{
}

Result Absolute::GetDelta( unify::TimeDelta & delta )
{
	delta = m_delta;
	return Result::Failure;
}
