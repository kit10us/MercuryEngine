// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/dyna/rotation/DR_Delta.h>

using namespace me;
using namespace dyna;
using namespace rotation;

Delta::Delta( IDynaRotation::ptr rotation, IDynaDelta::ptr delta )
	: m_rotation{ rotation }
	, m_delta{ delta }
{
}

Result Delta::GetRotation(unify::Quaternion & out, unify::TimeDelta delta)
{
	if( !m_rotation || !m_delta )
	{
		return Result::Failure;
	}

	if( m_delta->GetDelta( delta ) == Result::Failure )
	{
		return Result::Failure;
	}

	return m_rotation->GetRotation( out, delta );
}
