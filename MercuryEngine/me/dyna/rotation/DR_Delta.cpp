// Copyright (c) 2002 - 2013, Quentin S. Smith
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

Result Delta::GetRotation(unify::Quaternion & out, float delta)
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
