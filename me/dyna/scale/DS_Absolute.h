// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaScale.h>

namespace me::dyna::scale
{
	/// <summary>
	/// An absolute scale.
	/// </summary>
	class Absolute : public IDynaScale
	{
		unify::V3< float > m_v3;

	public:
		Absolute( unify::V3< float > v3 );

		Result GetScale( unify::V3< float >& out, unify::TimeDelta delta ) override;
	};
}