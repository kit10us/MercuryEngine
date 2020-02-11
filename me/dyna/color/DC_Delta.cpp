// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/dyna/color/DC_Delta.h>

using namespace me;
using namespace dyna;
using namespace color;

Delta::Delta( IDynaColor::ptr color, IDynaDelta::ptr delta )
	: m_color{ color }
	, m_delta{ delta }
{
}

Result Delta::GetColor(unify::ColorUnit & out, unify::TimeDelta delta)
{
	if( !m_color || !m_delta )
	{
		return Result::Failure;
	}

	if( m_delta->GetDelta( delta ) == Result::Failure )
	{
		return Result::Failure;
	}

	return m_color->GetColor( out, delta );
}
