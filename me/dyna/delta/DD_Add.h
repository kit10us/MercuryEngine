// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaDelta.h>

namespace me::dyna::delta
{
	/// <summary>
	/// Adds a scale to another position.
	/// </summary>
	class Add : public IDynaDelta
	{
		IDynaDelta::ptr m_deltaA;
	public:
		Add( IDynaDelta::ptr deltaA );

	public: // me::dyna::IDynaDelta...
		Result GetDelta( unify::TimeDelta & delta) override;
	};
}