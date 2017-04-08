// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/dyna/delta/DD_Mul.h>

using namespace me;
using namespace dyna;
using namespace delta;

Mul::Mul( IDynaDelta ::ptr deltaA )
	: m_deltaA{ deltaA }
{
}

Result Mul::GetDelta(float & delta)
{
	if ( m_deltaA )
	{
		return m_deltaA->GetDelta(delta);
	}

	return Result::Failure;
}
