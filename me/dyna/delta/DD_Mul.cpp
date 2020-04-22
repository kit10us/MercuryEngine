// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/dyna/delta/DD_Mul.h>

using namespace me::dyna::delta;
using namespace me::dyna;

Mul::Mul( IDynaDelta::ptr deltaA )
	: m_deltaA{ deltaA }
{
}

Result Mul::GetDelta( unify::TimeDelta & delta)
{
	return m_deltaA ? m_deltaA->GetDelta(delta) : Result::Failure;
}
