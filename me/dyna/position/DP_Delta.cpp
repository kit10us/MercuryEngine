// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/dyna/position/DP_Delta.h>

using namespace me::dyna;
using namespace me::dyna::position;

Delta::Delta( IDynaPosition::ptr position, IDynaDelta::ptr delta )
	: m_position{ position }
	, m_delta{ delta }
{
}

Result Delta::GetPosition(unify::V3< float >  & out, unify::TimeDelta delta)
{
	if( !m_position || !m_delta )
	{
		return Result::Failure;
	}

	if( m_delta->GetDelta( delta ) == Result::Failure )
	{
		return Result::Failure;
	}

	return m_position->GetPosition( out, delta );
}
