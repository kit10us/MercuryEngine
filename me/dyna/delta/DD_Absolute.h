// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaDelta.h>

namespace me::dyna::delta
{
	/// <summary>
	/// A IDynaDelta that's an absolute value.
	/// </summary>
	class Absolute : public IDynaDelta
	{
		unify::TimeDelta m_delta;
	public:
		Absolute( unify::TimeDelta delta );

	public: // me::dyna::delta...
		Result GetDelta( unify::TimeDelta & delta ) override;
	};
}