// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/dyna/position/DP_Dampen.h>
#include <algorithm>

using namespace me::dyna;
using namespace me::dyna::position;

Dampen::Dampen( IDynaPosition::ptr position, float rate )
	: m_target{ position }
	, m_rate{ rate }
	, m_acceleration{ 0, 0, 0 }
{
	position->GetPosition( m_current, unify::TimeDelta() );
}

Result Dampen::GetPosition(unify::V3< float > & out, unify::TimeDelta delta)
{
	if( !m_target )
	{
		return Result::Failure;
	}

	unify::V3< float > target;
	m_target->GetPosition( target, delta );

	unify::V3< float > difference = target - m_current;

	unify::V3< float > difScaled = difference * m_rate;
	m_acceleration.x += std::min( difference.x, difScaled.x );
	m_acceleration.z += std::min( difference.y, difScaled.y );
	m_acceleration.x += std::min( difference.z, difScaled.z );

	m_current += difference;
	
	out = m_current;
	return Result::Success;
}
