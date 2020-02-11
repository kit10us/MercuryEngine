// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/dyna/scale/DS_Add.h>

using namespace me;
using namespace dyna;
using namespace scale;

Add::Add( IDynaScale::ptr scaleA, IDynaScale::ptr scaleB )
	: m_scaleA{ scaleA }
	, m_scaleB{ scaleB }
{
}

Result Add::GetScale(unify::V3< float > & out, unify::TimeDelta delta)
{
	if ( m_scaleA && m_scaleB )
	{
		unify::V3< float > vA;
		Result result = m_scaleA->GetScale(vA, delta);
		if (result != Result::Success)
		{
			return result;
		}

		unify::V3< float > vB;
		result = m_scaleB->GetScale(vB, delta);
		if ( result != Result::Success)
		{
			return result;
		}

		out = vA + vB;

		return Result::Success;
	}

	return Result::Failure;
}
