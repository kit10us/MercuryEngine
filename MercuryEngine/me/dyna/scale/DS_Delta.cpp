// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/dyna/scale/DS_Delta.h>

using namespace me;
using namespace dyna;
using namespace scale;

Delta::Delta( IDynaScale::ptr scale, IDynaDelta::ptr delta )
	: m_scale{ scale }
	, m_delta{ delta }
{
}

Result Delta::GetScale(unify::V3< float > & out, float delta)
{
	if( !m_scale || !m_delta )
	{
		return Result::Failure;
	}

	if( m_delta->GetDelta( delta ) == Result::Failure )
	{
		return Result::Failure;
	}

	return m_scale->GetScale( out, delta );
}
