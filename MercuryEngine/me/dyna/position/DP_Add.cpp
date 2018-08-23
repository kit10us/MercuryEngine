// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/dyna/position/DP_Add.h>

using namespace me;
using namespace dyna;
using namespace position;

Add::Add( IDynaPosition::ptr positionA, IDynaPosition::ptr positionB )
	: m_positionA{ positionA }
	, m_positionB{ positionB }
{
}

Result Add::GetPosition(unify::V3< float > & out, unify::TimeDelta delta)
{
	if ( m_positionA && m_positionB )
	{
		unify::V3< float > vA;
		Result result = m_positionA->GetPosition(vA, delta);
		if (result != Result::Success)
		{
			return result;
		}

		unify::V3< float > vB;
		result = m_positionB->GetPosition(vB, delta);
		if ( result != Result::Success)
		{
			return result;
		}

		out = vA + vB;

		return Result::Success;
	}

	return Result::Failure;
}
