// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaPosition.h>

namespace me::dyna::position
{
	/// <summary>
	/// Dampens the change to a IDynaPosition.
	/// </summary>
	class Dampen : public IDynaPosition
	{
		IDynaPosition::ptr m_target;
		unify::V3< float > m_current;
		unify::V3< float > m_acceleration;
		float m_rate;

	public:
		Dampen( IDynaPosition::ptr position, float rate );

		Result GetPosition( unify::V3< float >& out, unify::TimeDelta delta ) override;
	};
}