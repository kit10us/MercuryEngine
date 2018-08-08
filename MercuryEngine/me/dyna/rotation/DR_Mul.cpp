// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/dyna/rotation/DR_Mul.h>

using namespace me;
using namespace dyna;
using namespace rotation;

Mul::Mul( IDynaRotation::ptr rotationA, IDynaRotation::ptr rotationB )
	: m_rotationA{ rotationA }
	, m_rotationB{ rotationB }
{
}

Result Mul::GetRotation(unify::Quaternion & out, float delta)
{
	if ( m_rotationA && m_rotationB )
	{
		unify::Quaternion qA;
		Result result = m_rotationA->GetRotation(qA, delta);
		if (result != Result::Success)
		{
			return result;
		}

		unify::Quaternion qB;
		result = m_rotationB->GetRotation(qB, delta);
		if ( result != Result::Success)
		{
			return result;
		}

		out = qA * qB;

		return Result::Success;
	}

	return Result::Failure;
}
