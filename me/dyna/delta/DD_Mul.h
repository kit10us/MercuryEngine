// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaDelta.h>

namespace me::dyna::delta
{
	/// <summary>
	/// Mulliplies a delta against a IDynaDeta.
	/// </summary>
	class Mul : public IDynaDelta
	{
		IDynaDelta::ptr m_deltaA;

	public:
		Mul( IDynaDelta::ptr deltaA );

	public: // me::dyna::delta::IDynaDelta...
		Result GetDelta( unify::TimeDelta & delta ) override;
	};
}