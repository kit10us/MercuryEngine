// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/dyna/rotation/DR_Add.h>

using namespace me;
using namespace dyna;
using namespace rotation;

Add::Add( IDynaRotation::ptr rotationA, IDynaRotation::ptr rotationB )
	: m_rotationA{ rotationA }
	, m_rotationB{ rotationB }
{
}

Result Add::GetRotation(unify::Quaternion & out, unify::TimeDelta delta)
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

		out = qA + qB;

		return Result::Success;
	}

	return Result::Failure;
}
