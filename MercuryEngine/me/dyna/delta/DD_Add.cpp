// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/dyna/delta/DD_Add.h>

using namespace me;
using namespace dyna;
using namespace delta;

Add::Add( IDynaDelta::ptr deltaA )
	: m_deltaA{ deltaA }
{
}

Result Add::GetDelta( unify::TimeDelta & delta)
{
	if ( m_deltaA )
	{
		return m_deltaA->GetDelta(delta);
	}

	return Result::Failure;
}
