// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/dyna/delta/DD_Add.h>

using namespace me::dyna::delta;
using namespace me::dyna;

Add::Add( IDynaDelta::ptr deltaA )
	: m_deltaA{ deltaA }
{
}

Result Add::GetDelta( unify::TimeDelta & delta)
{
	return m_deltaA ? m_deltaA->GetDelta(delta) : Result::Failure;
}
