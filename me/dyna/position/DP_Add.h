// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaPosition.h>

namespace me::dyna::position
{
	/// <summary>
	/// Adds a position to another position.
	/// </summary>
	class Add : public IDynaPosition
	{
		IDynaPosition::ptr m_positionA;
		IDynaPosition::ptr m_positionB;

	public:
		Add( IDynaPosition::ptr positionA, IDynaPosition::ptr positionB );

		Result GetPosition(unify::V3< float > & out, unify::TimeDelta delta) override;
	};
}
