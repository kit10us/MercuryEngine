// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaPosition.h>

namespace me::dyna::position
{
	/// <summary>
	/// An absolute position.
	/// </summary>
	class Absolute : public IDynaPosition
	{
		unify::V3< float > m_v3;
	public:
		Absolute(unify::V3< float > v3);

		Result GetPosition(unify::V3< float > & out, unify::TimeDelta delta) override;
	};
}